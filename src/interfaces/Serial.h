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

class Serial{

public:

	Serial();

	void Serial_init(const char* path, int baudrate);
	void Serial_close();

	void Serial_write(unsigned char* msg, int len);
	std::string Serial_read();

	void Lock();
	void Unlock();
	
private:

	int device;
	std::mutex mu;
	struct termios tio_new, tio_old;

};

#endif
