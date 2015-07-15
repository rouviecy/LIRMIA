/*
 * @(#)		SPI.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Provide SPI communication
 */

#ifndef SPI_H
#define SPI_H

#include <mutex>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>

class SPI{

public:

	SPI();

	#define SPI_BUFFER_LEN 32

	void SPI_init(const char* path, int speed);
	void SPI_close();

	void SPI_write(unsigned char* msg, int len);
	unsigned char* SPI_read(int len);

	void Lock();
	void Unlock();
	
private:

	unsigned char buf[SPI_BUFFER_LEN];
	int device;
	int speed;
	std::string path;
	std::mutex mu;

};

#endif
