#include "SPI.h"

using namespace std;

SPI::SPI(){}

void SPI::SPI_init(const char* path, int speed, int mode, bool little_endian, int bits_per_word){
	this->path = string(path);
	this->mode = mode;
	this->little_endian = little_endian;
	spi_transfer.speed_hz = speed;
	spi_transfer.delay_usecs = 0;
	spi_transfer.len = bits_per_word;
	device = open(path, O_RDWR);
        if(	device < 0){
			cout << "[Error] Unable to open SPI port to " + this->path << endl;
			return;
	}
	if(	ioctl(device, SPI_IOC_WR_MODE, &(this->mode)) < 0 or
		ioctl(device, SPI_IOC_RD_MODE, &(this->mode)) < 0){
			cout << "[Error] Failed to open SPI port to " + this->path + " : mode " + to_string(mode) + " is not available" << endl;
			return;
	}
	if(	ioctl(device, SPI_IOC_WR_BITS_PER_WORD, &(spi_transfer.len)) < 0 or
		ioctl(device, SPI_IOC_RD_BITS_PER_WORD, &(spi_transfer.len)) < 0){
			cout << "[Error] Failed to open SPI port to " + this->path + " : " + to_string(bits_per_word) + " bits per word is not available" << endl;
			return;
	}
	if(	ioctl(device, SPI_IOC_WR_LSB_FIRST, &(this->little_endian)) < 0 or
		ioctl(device, SPI_IOC_RD_LSB_FIRST, &(this->little_endian)) < 0){
			cout << "[Error] Failed to open SPI port to " + this->path + " : " + (little_endian ? "little endian":"big endian") + " is not available" << endl;
			return;
	}
	if(	ioctl(device, SPI_IOC_WR_MAX_SPEED_HZ, &(spi_transfer.speed_hz)) < 0 or
		ioctl(device, SPI_IOC_RD_MAX_SPEED_HZ, &(spi_transfer.speed_hz)) < 0){
			cout << "[Error] Failed to open SPI port to " + this->path + " : " + to_string(speed) + "Hz speed is not available" << endl;
			return;
	}
}

void SPI::SPI_close(){
	close(device);
}

void SPI::SPI_duplex(unsigned char* msg_in, unsigned char* msg_out){
	spi_transfer.rx_buf = (unsigned long) msg_in;
	spi_transfer.tx_buf = (unsigned long) msg_out;
	ioctl(device, SPI_IOC_MESSAGE(1), &spi_transfer);
}

void SPI::Lock(){mu.lock();}
void SPI::Unlock(){mu.unlock();}
