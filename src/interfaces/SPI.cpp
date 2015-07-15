#include "SPI.h"

using namespace std;

SPI::SPI(){}

void SPI::SPI_init(const char* path, int speed){
	this->path = string(path);
	this->speed = speed;
	device = open(path, O_RDWR);
        if(device < 0){
		cout << "[Error] Unable to open SPI port to " + this->path << endl;
		return;
	}
	if(ioctl(device, SPI_IOC_WR_MAX_SPEED_HZ, &(this->speed)) != 0){
                cout << "[Error] Unable to launch ioctl for SPI port to " + this->path << endl;
                return;
        }
}

void SPI::SPI_close(){
	close(device);
}

void SPI::SPI_write(unsigned char* msg, int len){
	write(device, msg, len);
}

unsigned char* SPI::SPI_read(int len){
	read(device, buf, len);
	return buf;
}

void SPI::Lock(){mu.lock();}
void SPI::Unlock(){mu.unlock();}
