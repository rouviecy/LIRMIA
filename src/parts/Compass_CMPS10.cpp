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
	int angle = ((int) answer[2] * 256 + (int) answer[3]);
	compass = (float) angle / 10.;
	serial->Unlock();
}

void Compass_CMPS10::Set_iss(Serial* serial){this->serial = serial;}
