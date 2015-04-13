/*
 * @(#)		TCP_server.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Provide TCP client
 * 	- First, call Configure(const char* server_ip, int server_port) to connect to server
 * 	- Call Send(string msg) to send your msg to server
 * 	- Call Receive() to wait a message from server
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
	void Close();

private:

	#define TCP_BUFFER_LEN 1024

	bool connected;
	int sock;
	char buffer[TCP_BUFFER_LEN];

};

#endif
