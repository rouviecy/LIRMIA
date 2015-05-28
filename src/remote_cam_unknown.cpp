#include "interfaces/TCP_client.h"
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <thread>
#include <mutex>

using namespace std;

void open_cam(const char* host, int port, cv::Mat* img_remote, char* key_stop, mutex* mu){
	TCP_client tcp_client;
	tcp_client.Configure(host, port);
	int nb_received_bytes = 0;
	while(*key_stop != 'q'){
		int img_size;
		char* tmp_size = tcp_client.Receive();
		try{
			img_size = stoi(tmp_size);
		}catch(std::exception const & e){
			continue;
		}
		if(img_size <= 0){continue;}
		unsigned char msg_in[img_size];
		for(int i = 0; i < img_size; i+= nb_received_bytes){
			nb_received_bytes = tcp_client.Direct_receive(msg_in + i, img_size - i);
			if(nb_received_bytes == -1){
				cout << "[Error] Bad image reception by TCP" << endl;
				continue;
			}
		}
		vector <unsigned char> msg_vect;
		for(int i = 0; i < img_size; i++){msg_vect.push_back(msg_in[i]);}
		mu->lock();
		try{
			*img_remote = cv::imdecode(msg_vect, CV_LOAD_IMAGE_COLOR);
		}catch(std::exception const & e){
			*img_remote = cv::Mat::zeros(240, 320, CV_8UC3);
			cout << "[Warning] Bad image reception : skip to next image" << endl;
		}
		mu->unlock();
		usleep(10000);
		if(*key_stop == 'a'){*key_stop = 'b';}
	}
	tcp_client.Close();
}

int main(int argc, char* argv[]){
	#if defined(ENABLE_TCP) and defined(ENABLE_TCPCAM)
		thread thr[4];
		mutex mu;
		char key = 'a'; char key1 = 'a';
		cv::Mat img_cam, img_blobs;
		const char* window_cam = "Unknown camera";	const char* window_blobs = "Unknown blobs";
		#ifdef ENABLE_CAM1
			cv::namedWindow(window_cam, cv::WINDOW_AUTOSIZE);			cv::moveWindow(window_cam, 800, 150);
			cv::namedWindow(window_blobs, cv::WINDOW_AUTOSIZE);			cv::moveWindow(window_blobs, 1200, 150);
			thr[0] = thread(open_cam, argv[1], 5243, &img_cam, &key1, &mu);		usleep(500000);
			thr[1] = thread(open_cam, argv[1], 5244, &img_blobs, &key1, &mu);	usleep(500000);
		#endif
		usleep(500000);
		while(key != 'q'){
			#ifdef ENABLE_CAM1
				if(key1 == 'b'){
					mu.lock();
					if(img_cam.size().width > 0 && img_cam.size().height > 0)	{cv::imshow(window_cam, img_cam);}
					if(img_blobs.size().width > 0 && img_blobs.size().height > 0)	{cv::imshow(window_blobs, img_blobs);}
					mu.unlock();
				}
			#endif
			key = cv::waitKey(10);
		}
		key1 = 'q';
		#ifdef ENABLE_CAM1
			thr[0].join();
			thr[1].join();
		#endif
	#else
		cout << "[Error] You need to activate TCP and TCPCAM to use remote camera" << endl;
	#endif
}
