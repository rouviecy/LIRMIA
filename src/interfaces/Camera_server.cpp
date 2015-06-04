#include "Camera_server.h"

using namespace std;

Camera_server::Camera_server(){
	enabled_record = false;
	nb_cams = 0;
	encode_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	encode_params.push_back(90);
}

Camera_server::~Camera_server(){
	Clear_all_flux();
	if(enabled_record){Disable_record();}
}

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

void Camera_server::Enable_record(int nb_cams){
	if(enabled_record){
		cout << "[Warning] Trying to enable camera video recording which is already enabled : abord mission" << endl;
		return;
	}
	enabled_record = true;
	cycle_index.clear();
	cycle_position.clear();
	img_buffers.clear();
	for(int i = 0; i < nb_cams; i++){
		cycle_index.push_back(-1);
		cycle_position.push_back(0);
	}
	time_t t_now = time(0);
	struct tm* now = localtime(&t_now);
	stringstream path_ss;
	path_ss	<< "./test/"
		<< (now->tm_year + 1900) << "-"
		<< setfill('0') << setw(2) << (now->tm_mon + 1) << "-"
		<< setfill('0') << setw(2) << now->tm_mday << "--"
		<< setfill('0') << setw(2) << now->tm_hour << ":"
		<< setfill('0') << setw(2) << now->tm_min << ":"
		<< setfill('0') << setw(2) << now->tm_sec << "--Video/";
	path_record = path_ss.str();
	string mkdir_command = "mkdir \"" + path_record + "\"";
	system(mkdir_command.c_str());
	for(int i = 0; i < nb_cams; i++){
		this->nb_cams = nb_cams;
		img_buffers.push_back(new cv::Mat[CAM_REC_BUFFER_SIZE]);
	}
}

void Camera_server::Disable_record(){
	if(!enabled_record){
		cout << "[Warning] Trying to disable camera video recording which is already disabled" << endl;
		return;
	}
	enabled_record = false;
	for(int i = 0; i < nb_cams; i++){
		delete[] img_buffers[i];
	}
	img_buffers.clear();
}

string Camera_server::Img_path_record(int cam_index, int img_index){
	stringstream img_path_ss;
	img_path_ss	<< path_record
			<< "CAM"
			<< cam_index
			<< "_"
			<< setfill('0') << setw(6) << img_index
			<< ".png";
	return img_path_ss.str();
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

void Camera_server::Record_img(cv::Mat img, int cam_index){
	if(!enabled_record){
		cout << "[Error] Trying to record camera video which is not enabled : abord mission" << endl;
		return;
	}
	if(cycle_position[cam_index] == CAM_REC_BUFFER_SIZE){
		cycle_position[cam_index] = 0;
		cycle_index[cam_index]++;
		for(int i = 0; i < CAM_REC_BUFFER_SIZE; i++){
			int index = cycle_index[cam_index] * CAM_REC_BUFFER_SIZE + i;
			cv::imwrite(Img_path_record(cam_index, index), img_buffers[cam_index][i]);
		}
	}
	img.copyTo(img_buffers[cam_index][cycle_position[cam_index]]);
	cycle_position[cam_index]++;
}

void Camera_server::Direct_record_img(cv::Mat img, std::string id){
	if(!enabled_record){
		cout << "[Error] Trying to record an image while recording is not enabled : abord mission" << endl;
		return;
	}
	cv::imwrite(path_record + id, img);
}
