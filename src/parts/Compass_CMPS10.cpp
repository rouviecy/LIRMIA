#include "Compass_CMPS10.h"

using namespace std;

Compass_CMPS10::Compass_CMPS10() : ComThread(){
	enable_iss = false;
	enable_i2c = false;
	compass = 0.;
	request_compass_iss[0] = 0x55;
	request_compass_iss[1] = 0xC1;
	request_compass_iss[2] = 0x00;
	request_compass_iss[3] = 0x04;
	request_compass_i2c[0] = 0x00;
}

Compass_CMPS10::~Compass_CMPS10(){}

void Compass_CMPS10::On_start(){}

void Compass_CMPS10::IO(){
	Link_output("compass",		COMFLOAT, 1, &compass);
	Link_output("compass_thxyz",	COMFLOAT, 3, compass_thxyz);
}

void Compass_CMPS10::Job(){
	#ifdef ENABLE_SERIAL_ISS
		if(enable_iss){
			serial->Lock();
			serial->Serial_write(request_compass_iss, 4);
			unsigned char* answer = (unsigned char*) serial->Serial_read();
			float angle_v1 = (float) ((int) answer[2] * 256 + (int) answer[3]) / 10;
			float angle_v2 = (float) ((int) answer[1] * 360) / 255;
			if(fabs(angle_v1 - angle_v2) < 5){
				compass = angle_v1;
			}
			serial->Unlock();
			Critical_send();
		}
	#endif

	#ifdef ENABLE_I2C
		if(enable_i2c){
			i2c->Lock();
			i2c->I2C_connect_device(CMPS10_I2C_ADDRESS);
			i2c->I2C_write(request_compass_i2c, 1);
			unsigned char* answer = i2c->I2C_read_uchar(21);
			float angle_v1 = (float) ((int) answer[2] * 256 + (int) answer[3]) / 10;
			float angle_v2 = (float) ((int) answer[1] * 360) / 255;
			if(fabs(angle_v1 - angle_v2) < 5){
				compass_thxyz[2] = angle_v1;
			}
			int answer_4 = (int) answer[4];
			int answer_5 = (int) answer[5];
			compass_thxyz[1] = (float) ((answer_4 > 128 ? -answer_4 : +answer_4) * 90) / 255;
			compass_thxyz[0] = (float) ((answer_5 > 128 ? -answer_5 : +answer_4) * 180) / 255;
			i2c->Unlock();
			Critical_send();
		}
	#endif
}

void Compass_CMPS10::Set_iss(Serial* serial){
	this->serial = serial;
	enable_iss = true;

}
void Compass_CMPS10::Set_i2c(I2C* i2c){
	#ifndef ENABLE_I2C
		cout << "[Warning] Trying to use CMPS10 without I2C enabled ; compass will be disabled" << endl;
		return;
	#endif
	this->i2c = i2c;
	enable_i2c = true;
}
