#include "./Camera_client.h"

using namespace std;

void Camera_client::Open_cam(thread_cam_t* thread_cam){
	TCP_client tcp_client;
	tcp_client.Configure(thread_cam->host, thread_cam->port);
	int nb_received_bytes = 0;
	while(thread_cam->key != 'q'){
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
		thread_cam->mu.lock();
		try{
			thread_cam->img = cv::imdecode(msg_vect, CV_LOAD_IMAGE_COLOR);
		}catch(std::exception const & e){
			thread_cam->img = cv::Mat::zeros(240, 320, CV_8UC3);
			cout << "[Warning] Bad image reception : skip to next image" << endl;
		}
		thread_cam->mu.unlock();
		if(thread_cam->key == 'a'){thread_cam->key = 'b';}
		usleep(10000);
	}
	tcp_client.Close();
}

void Camera_client::Subscribe_cam(string window_name, char* host, int port, int window_pos_x, int window_pos_y){
	#if not defined(ENABLE_TCP) or not defined(ENABLE_TCPCAM)
		cout << "[Error] You need to activate TCP and TCPCAM to use remote camera" << endl;
		return;
	#endif
	cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
	thread_cam_t* thread_cam = new thread_cam_t;
	thread_cam->key		= 'a';
	thread_cam->window_name	= window_name;
	thread_cam->host	= host;
	thread_cam->port	= port;
	thread_cam->thr		= thread(&(Camera_client::Open_cam), thread_cam);
	threads_cam.push_back(thread_cam);
	cv::moveWindow(window_name, window_pos_x, window_pos_y);
	usleep(500000);
}

void Camera_client::Show_video(){
	char key = 'a';
	while(key != 'q'){
		for(size_t i = 0; i < threads_cam.size(); i++){
			if(threads_cam[i]->key == 'b'){
				threads_cam[i]->mu.lock();
				if(threads_cam[i]->img.size().width > 0 && threads_cam[i]->img.size().height > 0){
					cv::imshow(threads_cam[i]->window_name, threads_cam[i]->img);
				}
				threads_cam[i]->mu.unlock();
			}
			
		}
		key = cv::waitKey(10);
	}
	for(size_t i = 0; i < threads_cam.size(); i++){
		threads_cam[i]->key = 'q';
		threads_cam[i]->thr.join();
		delete threads_cam[i];
	}
}
