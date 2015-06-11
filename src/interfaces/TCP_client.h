/*
 * @(#)		TCP_server.h
 * @version	1.1
 * @autor	C. Rouvière
 */

/**
 * Provide TCP client
 * 	- First, call Configure(const char* server_ip, int server_port) to connect to server
 * 	- Call Send(string msg) to send your msg to server
 * 	- Call Receive() to wait a message from server
 * 	- Call Direct_send(unsigned char* msg, int msg_size) to avoid string
 * 	- Call Direct_receive(unsigned char* msg, int msg_size) to avoid string ; it also returns length of incoming message
 * 	- Close() is called by destructor if you forget ...
 */

#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#ifndef TCP_CLIENT
#define TCP_CLIENT

class TCP_client{

public:

	TCP_client();
	~TCP_client();

	bool Configure(const char* server_ip, int server_port);
	char* Receive();
	void Send(std::string msg_out);
	void Direct_send(unsigned char* msg, int msg_size);
	int Direct_receive(unsigned char* msg, int msg_size);
	void Close();

private:

	#define TCP_BUFFER_LEN 1024

	bool connected;
	int sock;
	char buffer[TCP_BUFFER_LEN];

};

#endif
