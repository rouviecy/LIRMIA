#include "Motors.h"

using namespace std;

Motors::Motors() : ComThread(){
	#if defined(ENABLE_MOTORS) and (not defined(ENABLE_SERIAL_ISS) or not defined(ENABLE_I2C))
		cout << "[Warning] You are trying to use motors without serial and i2c : motors will be disabled" << endl;
	#endif
	#if defined(ENABLE_MOTORS) and not defined(ENABLE_SERIAL_ARDUINO)
		cout << "[Warning] You are trying to use rudder without arduino : rudder will be disabled" << endl;
	#endif
}

Motors::~Motors(){}

void Motors::On_start(){}

void Motors::IO(){
	Link_input("motor",		COMFLOAT, 1, &motor);
	Link_input("rudder",		COMFLOAT, 1, &rudder);
	Link_input("bow_thrusters",	COMFLOAT, 2, bow_thrusters);
}

void Motors::Job(){
	Critical_receive();
	int order_motor = min(abs((int) (255. * motor)), 255);
	int order_bow1 = min(abs((int) (255. * bow_thrusters[0])), 255);
	int order_bow2 = min(abs((int) (255. * bow_thrusters[1])), 255);
	int order_rudder = min(abs((int) (180. * rudder)), 180);
	Generate_order_i2c(0, order_motor, motor > 0);
	Generate_order_i2c(1, order_bow1, bow_thrusters[0] > 0);
	Generate_order_i2c(2, order_bow2, bow_thrusters[1] > 0);
	Generate_order_arduino(rudder > 0 ? +order_rudder : -order_rudder);
}

void Motors::Generate_order_i2c(int num_motor, int power, bool positive){
	#if defined(ENABLE_MOTORS) and defined(ENABLE_I2C) and defined(ENABLE_SERIAL_ISS)
		unsigned char order[8];
		switch(num_motor){
			case 0: order[1]= 0xB0;	break; // TODO : change addresses
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
		i2c->Lock();
		i2c->I2C_write(order, 8);
		i2c->Unlock();
	#endif
}

void Motors::Generate_order_arduino(int angle){
	#if defined(ENABLE_MOTORS) and defined(ENABLE_SERIAL_ARDUINO)
		// TODO : envoyer ordre gouvernail à l'arduino
	#endif
}

void Motors::Set_iss(Serial* serial){this->iss = serial;}
void Motors::Set_arduino(Serial* serial){this->arduino = serial;}
