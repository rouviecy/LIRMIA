#include "Motors.h"

using namespace std;

Motors::Motors() : ComThread(){
	#if defined(ENABLE_MOTORS) and not defined(ENABLE_SERIAL)
		cout << "[Warning] You are trying to use motors without serial : motors will be disabled" << endl;
	#endif
}

Motors::~Motors(){}

void Motors::On_start(){}

void Motors::IO(){
	Link_input("motor", COMFLOAT, 4, motor);
}

void Motors::Job(){
	Critical_receive();
	for(int i = 0; i < 4; i++){
		int order = min(abs((int) (255. * motor[i])), 255);
		if(order < 20){order = 0;}
		Generate_order(i, order, motor[i] > 0);
	}
}

void Motors::Generate_order(int num_motor, int power, bool positive){
	#if defined(ENABLE_MOTORS) and defined(ENABLE_SERIAL)
		unsigned short target = 5777 + (positive ? +1 : -1) * (unsigned short) power * 7;
		unsigned char cmd[] = {0x84, (unsigned char) num_motor, (unsigned char) (target & 0x7F), (unsigned char) (target >> 7 & 0x7F)};
		serial->Lock();
		serial->Serial_write(cmd, 4);
		serial->Unlock();
	#endif
}

void Motors::Set_serial(Serial* serial){this->serial = serial;}
