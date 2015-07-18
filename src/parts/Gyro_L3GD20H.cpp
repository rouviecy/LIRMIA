#include "Gyro_L3GD20H.h"

using namespace std;

Gyro_L3GD20H::Gyro_L3GD20H() : ComThread(){
	request_gyro_i2c[0] = 0x2C;
	request_gyro_i2c[1] = 0x2D;
}

Gyro_L3GD20H::~Gyro_L3GD20H(){}

void Gyro_L3GD20H::On_start(){}

void Gyro_L3GD20H::IO(){
	Link_output("gyro_vthxyz",	COMFLOAT, 3, gyro_vthxyz);
}

void Gyro_L3GD20H::Job(){
	#ifdef ENABLE_I2C
		i2c->Lock();
		i2c->I2C_connect_device(L3GD20H_I2C_ADDRESS);
		i2c->I2C_write(&(request_gyro_i2c[0]), 1);
		short big = (short) *(i2c->I2C_read_uchar(1));
		i2c->I2C_write(&(request_gyro_i2c[1]), 1);
		short little = (short) *(i2c->I2C_read_uchar(1));
		gyro_vthxyz[2] = (float) (big << 8 | little);
		i2c->Unlock();
		Critical_send();
	#endif
}

void Gyro_L3GD20H::Set_i2c(I2C* i2c){
	#ifndef ENABLE_I2C
		cout << "[Warning] Trying to use L3GD20H without I2C enabled ; gyro will be disabled" << endl;
		return;
	#endif
	this->i2c = i2c;
}
