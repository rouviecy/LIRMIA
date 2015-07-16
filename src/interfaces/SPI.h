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

	void SPI_init(const char* path, int speed, int mode, bool little_endian, int bits_per_word);
	void SPI_close();

	void SPI_duplex(unsigned char* msg_in, unsigned char* msg_out);

	void Lock();
	void Unlock();

private:

	int device;
	int mode;
	bool little_endian;
	spi_ioc_transfer spi_transfer;
	std::string path;
	std::mutex mu;

};

#endif
