#include "Motors.h"

using namespace std;

Motors::Motors() : ComThread(){
	#if defined(ENABLE_MOTORS) and (not defined(ENABLE_SERIAL_ISS) or not defined(ENABLE_I2C))
		cout << "[Warning] You are trying to use motors without serial and i2c : motors will be disabled" << endl;
	#endif
}

Motors::~Motors(){}

void Motors::On_start(){}

void Motors::IO(){
	Link_input("motor1", &motor1);
	Link_input("motor2", &motor2);
	Link_input("motor3", &motor3);
	Link_input("motor4", &motor4);
}

void Motors::Job(){
	Critical_receive();
	int order1 = min(abs((int) (255. * motor1)), 255); if(order1 < 20){order1 = 0;}
	int order2 = min(abs((int) (255. * motor2)), 255); if(order2 < 20){order2 = 0;}
	int order3 = min(abs((int) (255. * motor3)), 255); if(order3 < 20){order3 = 0;}
	int order4 = min(abs((int) (255. * motor4)), 255); if(order4 < 20){order4 = 0;}
	Generate_order(1, order1, motor1 > 0);
	Generate_order(2, order2, motor2 > 0);
	Generate_order(3, order3, motor3 > 0);
	Generate_order(4, order4, motor4 > 0);
	cout << endl;
}

void Motors::Generate_order(int num_motor, int power, bool positive){
	cout << "Motor" << num_motor << " : " << (positive ? "+" : "-") << power << endl;
	#if defined(ENABLE_MOTORS) and defined(ENABLE_I2C) and defined(ENABLE_SERIAL_ISS)
		unsigned char order[8];
		switch(num_motor){
			case 1: order[1]= 0xB0;	break;
			case 2: order[1]= 0xB2;	break;
			case 3: order[1]= 0xB4;	break;
			case 4: order[1]= 0xB6;	break;
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

void Motors::Set_i2c(I2C* i2c){this->i2c = i2c;}
