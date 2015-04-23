#include "TCP_client.h"

using namespace std;

TCP_client::TCP_client(){
	connected = false;
}

TCP_client::~TCP_client(){
	if(connected){Close();}
}

bool TCP_client::Configure(const char* server_ip, int server_port){
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		cout << "[Error] Failed to open TCP client socket on " + string(server_ip) + ":" + to_string(server_port) << endl;
		return false;
	}
	struct hostent *hostinfo = gethostbyname(server_ip);
	if(hostinfo == NULL){
		cout << "[Error] Unknown host " + string(server_ip) << endl;
		return false;
	}
	sockaddr_in sin = { 0 };
	sin.sin_family = AF_INET;
	sin.sin_addr = *(in_addr*) hostinfo->h_addr;
	sin.sin_port = htons(server_port);
	if(connect(sock,(sockaddr*) &sin, sizeof(sockaddr)) < 0){
		cout << "[Error] Failed to connect TCP client socket on " + string(server_ip) + ":" + to_string(server_port) << endl;
		return false;
	}
	cout << "TCP client connected on " + string(server_ip) + ":" + to_string(server_port) << endl;
	connected = true;
	return true;
}

char* TCP_client::Receive(){
	if(!connected){
		cout << "[Warning] Trying to receive message but TCP client is not connected !" << endl;
		return NULL;
	}
	recv(sock, buffer, TCP_BUFFER_LEN, 0);
	return buffer;
}

void TCP_client::Send(string msg_out){
	if(!connected){
		cout << "[Warning] Trying to send message but TCP client is not connected !" << endl;
		return;
	}
	char* msg = new char[TCP_BUFFER_LEN];
	copy(msg_out.begin(), msg_out.end(), msg);
	msg[msg_out.size()] = '\0';
	send(sock, msg, TCP_BUFFER_LEN, 0);
	delete[] msg;
}

void TCP_client::Direct_send(unsigned char* msg, int msg_size){
	send(sock, msg, msg_size, 0);
}

int TCP_client::Direct_receive(unsigned char* msg, int msg_size){
	return recv(sock, msg, msg_size, 0);
}

void TCP_client::Close(){
	if(!connected){
		cout << "[Warning] Trying to close TCP client which is not connected !" << endl;
		return;
	}
	else{
		close(sock);
	}
	connected = false;
}
