/*
 * @(#)		Serial.h
 * @version	3.0
 * @autor	C. Rouvière
 */

/**
 * Provide serial communication
 */

#ifndef SERIAL
#define SERIAL

#include <mutex>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <iostream>

class Serial{

public:

	#define SERIAL_BUFFER_LEN 128

	Serial();

	void Serial_init(const char* path, int baudrate, bool canonical_mode);
	void Serial_close();

	void Serial_write(unsigned char* msg, int len);
	char* Serial_read();

	void Lock();
	void Unlock();
	
private:

	char buf[SERIAL_BUFFER_LEN];
	int device;
	std::mutex mu;
	struct termios tio_new, tio_old;

	std::string path;
	int baudrate;
	bool canonical_mode;

};

#endif
