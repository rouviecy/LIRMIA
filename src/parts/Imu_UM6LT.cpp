#include "Imu_UM6LT.h"

using namespace std;

Imu_UM6LT::Imu_UM6LT() : ComThread(){
	imu_thxyz[0] = 0.;
	imu_thxyz[1] = 0.;
	imu_thxyz[2] = 0.;
}

Imu_UM6LT::~Imu_UM6LT(){}

void Imu_UM6LT::On_start(){}

void Imu_UM6LT::IO(){
	Link_output("imu_thxyz", COMFLOAT, 3, imu_thxyz);
}

void Imu_UM6LT::Job(){
	#ifdef ENABLE_I2C
		i2c->Lock();
		unsigned char* answer = i2c->I2C_read_uchar(10);
		for(int i = 0; i < 10; i++){
			cout << (int) answer[i] << " | ";
		}
		cout << endl;
		i2c->Unlock();
		Critical_send();
	#endif
}

void Imu_UM6LT::Set_i2c(I2C* i2c){
	#ifndef ENABLE_I2C
		cout << "[Warning] Trying to use UM6-LT without I2C enabled ; IMU will be disabled" << endl;
		return;
	#endif
	this->i2c = i2c;
}
