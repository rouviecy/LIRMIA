#include "interfaces/TCP_client.h"
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char* argv[]){
	#ifdef ENABLE_TCPCAM
		TCP_client tcp_client;
		tcp_client.Configure(argv[1], stoi(argv[2]));
		string window_name = string(argv[1]) + ":" + string(argv[2]);
		int nb_received_bytes = 0;
		while(true){
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
			cv::Mat img_remote = cv::imdecode(msg_vect, CV_LOAD_IMAGE_COLOR);
			cv::imshow(window_name, img_remote);
			cv::waitKey(10);
		}
		tcp_client.Close();
	#else
		cout << "[Error] You need to activate TCPCAM to use remote camera" << endl;
	#endif
}
