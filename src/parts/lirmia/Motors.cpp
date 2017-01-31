#include "Motors.h"

using namespace std;

Motors::Motors() : ComThread(){
	#if defined(ENABLE_MOTORS) and not defined(ENABLE_SERIAL_ISS)
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
		int order = min(abs((int) (255. * motor[i])), 186);
		if(order < 20){order = 0;}
		Generate_order(i, order, motor[i] > 0);
	}
}

void Motors::Generate_order(int num_motor, int power, bool positive){
	#if defined(ENABLE_MOTORS) and defined(ENABLE_SERIAL_ISS)
		unsigned char order[8];
		switch(num_motor){
			case 0: order[1]= 0xB0;	break;
			case 1: order[1]= 0xB2;	break;
			case 2: order[1]= 0xB4;	break;
			case 3: order[1]= 0xB6;	break;
			default:		return;
		}
		order[0] = 0x55;
		order[2] = 0x00;
		order[3] = 0x04;
		order[4] = positive ? 1 : 2;
		order[5] = 0x00;
		order[6] = power;
		order[7] = 2;
		serial->Lock();
		serial->Serial_write(order, 8);
cout << "[DEBUG] Order to motor n°" << num_motor << " : " << (positive ? "+" : "-") << power << endl;
		serial->Unlock();
	#endif
}

void Motors::Set_serial(Serial* serial){this->serial = serial;}
