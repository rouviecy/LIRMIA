#include "interfaces/TCP_client.h"
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <thread>

using namespace std;

void open_cam(const char* host, int port, cv::Mat* img_remote, char* key_stop){
	TCP_client tcp_client;
	tcp_client.Configure(host, port);
	int nb_received_bytes = 0;
	while(*key_stop != 'q'){
		int img_size = stoi(tcp_client.Receive());
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
		*img_remote = cv::imdecode(msg_vect, CV_LOAD_IMAGE_COLOR);
		usleep(10000);
		if(*key_stop == 'a'){*key_stop = 'b';}
	}
	tcp_client.Close();
}

int main(int argc, char* argv[]){
	#if defined(ENABLE_TCP) and defined(ENABLE_TCPCAM)
		thread thr[4];
		char key = 'a'; char key1 = 'a'; char key2 = 'a';
		cv::Mat img_cam1, img_blobs1, img_cam2, img_blobs2;
		#ifdef ENABLE_CAM1
			thr[0] = thread(open_cam, argv[1], 4243, &img_cam1, &key1);
			usleep(2000000);
			thr[1] = thread(open_cam, argv[1], 4245, &img_blobs1, &key1);
			usleep(2000000);
		#endif
		#ifdef ENABLE_CAM2
			thr[2] = thread(open_cam, argv[1], 4244, &img_cam2, &key2);
			usleep(2000000);
			thr[3] = thread(open_cam, argv[1], 4246, &img_blobs2, &key2);
			usleep(2000000);
		#endif
		while(key != 'q'){
			#ifdef ENABLE_CAM1
				if(key1 == 'b'){
					cv::imshow("camera 1", img_cam1);
					cv::imshow("blobs 1", img_blobs1);
				}
			#endif
			#ifdef ENABLE_CAM2
				if(key2 == 'b'){
					cv::imshow("camera 2", img_cam2);
					cv::imshow("blobs 2", img_blobs2);
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
