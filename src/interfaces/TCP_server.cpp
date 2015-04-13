#include "TCP_server.h"

using namespace std;

TCP_server::TCP_server(){
	connected = false;
}

TCP_server::~TCP_server(){
	if(connected){Close();};
}

void TCP_server::Configure(int server_port){
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		cout << "[Error] Failed to open TCP server socket on port " + to_string(server_port) << endl;
		return;
	}
	this->server_port = server_port;
	struct sockaddr_in sin = { 0 };
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(server_port);
	if(bind(sock, (sockaddr*) &sin, sizeof sin) < 0){
		cout << "[Error] Failed to bind TCP server socket on port " + to_string(server_port) << endl;
		return;
	}
	cout << "TCP server connected on " + to_string(server_port) << endl;
	thr = thread(&TCP_server::Accept_new_clients, this);
}

void TCP_server::Accept_new_clients(){
	connected = true;
	while(connected){
		if(listen(sock, 5) < 0){
			cout << "[Error] Failed to listen to TCP server socket on port " + to_string(server_port) << endl;
			return;
		}
		sockaddr_in csin = { 0 };
		socklen_t sinsize = sizeof csin;
		int csock = accept(sock, (sockaddr*) &csin, &sinsize);
		if(csock < 0){
			cout << "[Error] Failed to create client socket for TCP server socket on port " + to_string(server_port) << endl;
			return;
		}
		clients.push_back(csock);
	}
}

char* TCP_server::Receive(int client_index){
	if(!connected){
		cout << "[Warning] Trying to send message but TCP server is not connected !" << endl;
		return NULL;
	}
	if(client_index >= 0 && client_index < clients.size()){
		recv(clients[client_index], buffer, TCP_BUFFER_LEN, 0);
		return buffer;
	}
	else{
		cout << "[Warning] Bad client_index ; abord receive on TCP server on port " + to_string(server_port) << endl;
		return NULL;
	}
}

void TCP_server::Send(string msg_out){
	if(!connected){
		cout << "[Warning] Trying to send message but TCP server is not connected !" << endl;
		return;
	}
	char* msg = new char[TCP_BUFFER_LEN];
	copy(msg_out.begin(), msg_out.end(), msg);
	msg[msg_out.size()] = '\0';
	for(int i = 0; i < clients.size(); i++){
		send(clients[i], msg, TCP_BUFFER_LEN, 0);
	}
	delete[] msg;
}

void TCP_server::Close(){
	if(!connected){
		cout << "[Warning] Trying to close TCP server which is not connected !" << endl;
		return;
	}
	connected = false;
	for(int i = 0; i < clients.size(); i++){
		close(clients[i]);
	}
	close(sock);
	thr.join();
	usleep(1000000);
	clients.clear();
}

int TCP_server::Get_nb_clients(){return clients.size();}
