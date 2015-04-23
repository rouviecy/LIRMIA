#include "interfaces/TCP_client.h"
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char* argv[]){
	#ifdef ENABLE_TCPCAM
		cv::Mat img_remote = cv::Mat::zeros(240, 320, CV_8UC3);
		int img_size = img_remote.total() * img_remote.elemSize();
		unsigned char msg_in[img_size];
		int nb_received_bytes = 0;
		TCP_client tcp_client;
		tcp_client.Configure(argv[1], stoi(argv[2]));
		while(true){
			img_remote = cv::Mat::zeros(240, 320, CV_8UC3);
			for(int i = 0; i < img_size; i+= nb_received_bytes){
				nb_received_bytes = tcp_client.Direct_receive(msg_in + i, img_size - i);
				if(nb_received_bytes == -1){
					cout << "[Error] Bad image reception by TCP" << endl;
					continue;
				}
				
			}
			int pt = 0;
			for(int i = 0; i < img_remote.rows; i++){
				for(int j = 0; j < img_remote.cols; j++){                                     
					img_remote.at <cv::Vec3b> (i, j) = cv::Vec3b(msg_in[pt + 0], msg_in[pt + 1], msg_in[pt + 2]);
					pt += 3;
				}
			}
			cv::imshow("test", img_remote);
			cv::waitKey(10);
		}
		tcp_client.Close();
	#else
		cout << "[Error] You need to activate TCPCAM to use remote camera" << endl;
	#endif
}
