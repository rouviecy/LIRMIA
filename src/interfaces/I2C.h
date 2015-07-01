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
#include "./i2c-dev.h"

class I2C{

public:

	I2C();

	#define I2C_BUFFER_LEN 16

	void I2C_init(const char* path);
	void I2C_connect_device(int address);
	void I2C_close();

	void I2C_write(unsigned char* msg, int len);
	void I2C_write_ushort_address(int address, unsigned short value);
	unsigned char* I2C_read_uchar(int len);
	unsigned char* I2C_read_uchar_address(int address, int len);
	unsigned short* I2C_read_ushort_address(int address);

	void Lock();
	void Unlock();
	
private:

	unsigned char buf8[I2C_BUFFER_LEN];
	unsigned short buf16[I2C_BUFFER_LEN];
	int device;
	std::string path;
	std::mutex mu;

};

#endif
