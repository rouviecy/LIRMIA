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
		mutex mu1, mu2;
		char key = 'a'; char key1 = 'a'; char key2 = 'a';
		cv::Mat img_cam1, img_blobs1, img_cam2, img_blobs2;
		const char* window_cam1 = "Lirmia camera 1";	const char* window_blobs1 = "Lirmia blobs 1";
		const char* window_cam2 = "Lirmia camera 2";	const char* window_blobs2 = "Lirmia blobs 2";
		#ifdef ENABLE_CAM1
			cv::namedWindow(window_cam1, cv::WINDOW_AUTOSIZE);	cv::moveWindow(window_cam1, 800, 150);
			cv::namedWindow(window_blobs1, cv::WINDOW_AUTOSIZE);	cv::moveWindow(window_blobs1, 1200, 150);
			thr[0] = thread(open_cam, argv[1], 4243, &img_cam1, &key1, &mu1);	usleep(500000);
			thr[1] = thread(open_cam, argv[1], 4245, &img_blobs1, &key1, &mu1);	usleep(500000);
		#endif
		#ifdef ENABLE_CAM2
			cv::namedWindow(window_cam2, cv::WINDOW_AUTOSIZE);	cv::moveWindow(window_cam2, 800, 500);
			cv::namedWindow(window_blobs2, cv::WINDOW_AUTOSIZE);	cv::moveWindow(window_blobs2, 1200, 500);
			thr[2] = thread(open_cam, argv[1], 4244, &img_cam2, &key2, &mu2);	usleep(500000);
			thr[3] = thread(open_cam, argv[1], 4246, &img_blobs2, &key2, &mu2);	usleep(500000);
		#endif
		usleep(500000);
		while(key != 'q'){
			#ifdef ENABLE_CAM1
				if(key1 == 'b'){
					mu1.lock();
					if(img_cam1.size().width > 0 && img_cam1.size().height > 0)	{cv::imshow(window_cam1, img_cam1);}
					if(img_blobs1.size().width > 0 && img_blobs1.size().height > 0)	{cv::imshow(window_blobs1, img_blobs1);}
					mu1.unlock();
				}
			#endif
			#ifdef ENABLE_CAM2
				if(key2 == 'b'){
					mu2.lock();
					if(img_cam2.size().width > 0 && img_cam2.size().height > 0)	{cv::imshow(window_cam2, img_cam2);}
					if(img_blobs2.size().width > 0 && img_blobs2.size().height > 0)	{cv::imshow(window_blobs2, img_blobs2);}
					mu2.unlock();
				}
			#endif
			key = cv::waitKey(10);
		}
		key1 = 'q'; key2 = 'q';
		#ifdef ENABLE_CAM1
			thr[0].join();
			thr[1].join();
		#endif
		#ifdef ENABLE_CAM2
			thr[2].join();
			thr[3].join();
		#endif
	#else
		cout << "[Error] You need to activate TCP and TCPCAM to use remote camera" << endl;
	#endif
}
