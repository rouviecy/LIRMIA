#include "Camera_server.h"

using namespace std;

Camera_server::Camera_server(){}

Camera_server::~Camera_server(){Clear_all_flux();}

bool Camera_server::Add_flux(int port){
	#ifdef ENABLE_TCP
		if(tcp_servers.count(port) > 0){
			cout << "[Error] Trying to add TCP camera flux to port " << to_string(port) << " which is already used" << endl;
			return false;
		}
		TCP_server* new_server = new TCP_server();
		if(!new_server->Configure(port)){
			cout << "[Error] Failed to open TCP camera server on port " << to_string(port) << endl;
			return false;
		}
		cout << "Waiting client for TCP camera on port " << to_string(port) << " ..." << endl;
		while(new_server->Get_nb_clients() == 0){
			usleep(200000);
		}
		cout << "Client for TCP camera connected on port " << to_string(port) << endl;
		tcp_servers[port] = new_server;
		return true;
	#else
		cout << "[Warning] Trying to open TCP camera server with TCP disabled" << endl;
		return false;
	#endif
}

void Camera_server::Clear_all_flux(){
	#ifdef ENABLE_TCP
		for(map <int, TCP_server*> ::iterator it = tcp_servers.begin(); it != tcp_servers.end(); ++it){
			it->second->Close();
			delete it->second;
		}
		tcp_servers.clear();
	#endif
}

bool Camera_server::Send_tcp_img(cv::Mat img, int port){
	#ifdef ENABLE_TCP
		if(tcp_servers.count(port) == 0){
			cout << "[Error] Cannot send image to TCP port " << to_string(port) << " which is not opened" << endl;
			return false;
		}
		cv::Mat img_mini;
		std::vector <unsigned char> msg;
		cv::resize(img, img_mini, cv::Size(320, 240));
		std::vector<int> encode_params;
		encode_params.push_back(CV_IMWRITE_JPEG_QUALITY);
		encode_params.push_back(90);
		cv::imencode(".jpg", img_mini, msg, encode_params);
		tcp_servers[port]->Send(to_string(msg.size()));
		unsigned char msg_char[msg.size()];
		for(int i = 0; i < msg.size(); i++){
			msg_char[i] = msg[i];
		}
		tcp_servers[port]->Direct_send(msg_char, msg.size());
		return true;
	#endif
}
