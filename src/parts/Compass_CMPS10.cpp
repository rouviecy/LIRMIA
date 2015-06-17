#include "Compass_CMPS10.h"

using namespace std;

Compass_CMPS10::Compass_CMPS10() : ComThread(){
	compass = 0.;
	request_compass[0] = 0x55;
	request_compass[1] = 0xC1;
	request_compass[2] = 0x00;
	request_compass[3] = 0x04;
}

Compass_CMPS10::~Compass_CMPS10(){}

void Compass_CMPS10::On_start(){}

void Compass_CMPS10::IO(){
	Link_output("compass",	COMFLOAT, 1, &compass);
}

void Compass_CMPS10::Job(){
	#ifndef ENABLE_SERIAL_ISS
		return;
	#endif
	serial->Lock();
	serial->Serial_write(request_compass, 4);
	char* answer = serial->Serial_read();
	float angle_v1 = (float) ((int) answer[2] * 256 + (int) answer[3]) / 10;
	float angle_v2 = (float) ((int) answer[1] * 360) / 255;
	if(fabs(angle_v1 - angle_v2) < 5){
		compass = angle_v1;
	}
	serial->Unlock();
	Critical_send();
}

void Compass_CMPS10::Set_iss(Serial* serial){this->serial = serial;}
