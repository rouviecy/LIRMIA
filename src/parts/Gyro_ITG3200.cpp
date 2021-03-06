#include "Gyro_ITG3200.h"

using namespace std;

Gyro_ITG3200::Gyro_ITG3200() : ComThread(){
	request_gyro_z_high = 0x21;
	request_gyro_z_low = 0x22;
}

Gyro_ITG3200::~Gyro_ITG3200(){}

void Gyro_ITG3200::On_start(){}

void Gyro_ITG3200::IO(){
	Link_output("gyro_vthxyz",	COMFLOAT, 3, gyro_vthxyz);
}

void Gyro_ITG3200::Job(){
	#ifdef ENABLE_I2C
		i2c->Lock();
		i2c->I2C_connect_device(ITG3200_I2C_ADDRESS);
		i2c->I2C_write(&request_gyro_z_high, 1);
		unsigned char answer1 = *(i2c->I2C_read_uchar(1));
		i2c->I2C_write(&request_gyro_z_low, 1);
		unsigned char answer2 = *(i2c->I2C_read_uchar(1));
		short data_high = answer1 < 128 ? (short) answer1 : (short) answer1 - 256;
		short data_low = answer2 < 128 ? (short) answer2 : (short) answer2 - 256;
		gyro_vthxyz[2] = (float) (data_high << 8 | data_low);
		i2c->Unlock();
		Critical_send();
	#endif
}

void Gyro_ITG3200::Set_i2c(I2C* i2c){
	#ifndef ENABLE_I2C
		cout << "[Warning] Trying to use ITG3200 without I2C enabled ; gyro will be disabled" << endl;
		return;
	#endif
	this->i2c = i2c;
}
