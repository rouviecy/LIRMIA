/*
 * @(#)		I2C.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Provide I2C communication
 */

#ifndef I2C_H
#define I2C_H

#include <mutex>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

class I2C{

public:

	I2C();

	#define I2C_BUFFER_LEN 16

	void I2C_init(const char* path);
	void I2C_connect_device(int address);
	void I2C_close();

	void I2C_write(unsigned char* msg, int len);
	unsigned char* I2C_read(int len);

	void Lock();
	void Unlock();
	
private:

	unsigned char buf[I2C_BUFFER_LEN];
	int device;
	std::string path;
	std::mutex mu;

};

#endif
