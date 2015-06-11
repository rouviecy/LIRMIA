/*
 * @(#)		TCP_server.h
 * @version	1.1
 * @autor	C. Rouvière
 */

/**
 * Provide TCP server
 * 	- First, call Configure(int server_port) to open socket and wait clients
 * 	- Call Send(string msg) to broadcast your msg to every clients
 * 	- Call Receive(int client_index) to wait a message from client with this index
 * 	- Call Direct_send(unsigned char* msg, int msg_size) to avoid string
 * 	- Call Direct_receive(int client_index, unsigned char* msg, int msg_size) to avoid string ; it also returns length of incoming message
 * 	- Close() is called by destructor if you forget ...
 */

#include <vector>
#include <thread>
#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "TCP_client.h"

#ifndef TCP_SERVER
#define TCP_SERVER

class TCP_server{

public:

	TCP_server();
	~TCP_server();

	bool Configure(int server_port);
	int Get_nb_clients();
	char* Receive(int client_index);
	void Send(std::string msg_out);
	void Direct_send(unsigned char* msg, int msg_size);
	int Direct_receive(int client_index, unsigned char* msg, int msg_size);
	void Close();

private:

	#define TCP_BUFFER_LEN 1024

	std::thread thr;
	bool connected;
	int server_port;
	int sock;
	std::vector <int> clients;
	char buffer[TCP_BUFFER_LEN];

	void Accept_new_clients();

};

#endif
