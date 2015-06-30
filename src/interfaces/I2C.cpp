#include "I2C.h"

using namespace std;

I2C::I2C(){}

void I2C::I2C_init(const char* path){
	this->path = string(path);
	device = open(path, O_RDWR);
        if(device < 0){
		cout << "[Error] Unable to open I2C port to " + this->path << endl;
		return;
	}
}

void I2C::I2C_connect_device(int address){
	if(ioctl(device, I2C_SLAVE, address) < 0){
		cout << "[Error] Unable to find the device " + to_string(address) + " in I2C port " + this->path << endl;
	}
}

void I2C::I2C_close(){
	close(device);
}

void I2C::I2C_write(unsigned char* msg, int len){
	write(device, msg, len);
}

unsigned char* I2C::I2C_read(int len){
	read(device, buf, len);
	return buf;
}

void I2C::Lock(){mu.lock();}
void I2C::Unlock(){mu.unlock();}
