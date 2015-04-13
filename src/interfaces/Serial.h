/*
 * @(#)		Serial.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Provide serial communication
 */

#ifndef SERIAL
#define SERIAL

#include <string>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

class Serial{

public:

	Serial();

	void Serial_init(const char* path);
	void Serial_close();

	void Serial_write(std::string msg);
	std::string Serial_read();
	
private:

	int device;
	struct termios tio_new, tio_old;

};

#endif
