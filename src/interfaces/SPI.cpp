#include "SPI.h"

using namespace std;

SPI::SPI(){}

void SPI::SPI_init(const char* path, int speed){
	this->path = string(path);
	this->speed = speed;
	uint8_t mode = 0;
	uint8_t bits = 8;
	uint8_t lsb_first = 0;
//	mode &= ~SPI_CPOL;
//	mode &= ~SPI_CPHA;
	device = open(path, O_RDWR);
        if(device < 0){
		cout << "[Error] Unable to open SPI port to " + this->path << endl;
		return;
	}
	int ok = 1;
	ok = min(ok, ioctl(device, SPI_IOC_WR_MODE, &mode));
	ok = min(ok, ioctl(device, SPI_IOC_RD_MODE, &mode));
	ok = min(ok, ioctl(device, SPI_IOC_WR_BITS_PER_WORD, &bits));
	ok = min(ok, ioctl(device, SPI_IOC_RD_BITS_PER_WORD, &bits));
	ok = min(ok, ioctl(device, SPI_IOC_WR_LSB_FIRST, &lsb_first));
	ok = min(ok, ioctl(device, SPI_IOC_RD_LSB_FIRST, &lsb_first));
	ok = min(ok, ioctl(device, SPI_IOC_WR_MAX_SPEED_HZ, &(this->speed)));
	ok = min(ok, ioctl(device, SPI_IOC_RD_MAX_SPEED_HZ, &(this->speed)));
	cout << ok << endl;
}

void SPI::SPI_close(){
	close(device);
}

void SPI::SPI_write(unsigned char* msg, int len){
	write(device, msg, len);
}

void SPI::SPI_duplex(unsigned char* msg, int len){
	cout << ioctl(device, SPI_IOC_MESSAGE(1), msg) << endl;
}

unsigned char* SPI::SPI_read(int len){
	read(device, buf, len);
	return buf;
}

void SPI::Lock(){mu.lock();}
void SPI::Unlock(){mu.unlock();}
