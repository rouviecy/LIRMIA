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

void I2C::I2C_write_ushort_address(int address, unsigned short value){
	i2c_smbus_write_byte_data(device, address, value);
}

unsigned char* I2C::I2C_read_uchar(int len){
	read(device, buf8, len);
	return buf8;
}

unsigned char* I2C::I2C_read_uchar_address(int address, int len){
	i2c_smbus_read_i2c_block_data(device, address, len, buf8);
	return buf8;
}

unsigned short* I2C::I2C_read_ushort_address(int address){
	*buf16 = i2c_smbus_read_word_data(device, address);
	return buf16;
}

void I2C::Lock(){mu.lock();}
void I2C::Unlock(){mu.unlock();}
