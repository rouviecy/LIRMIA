#include "Motors.h"

using namespace std;

Motors::Motors() : ComThread(){
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
	int order1 = min(abs((int) (255. * motor1)), 255);
	int order2 = min(abs((int) (255. * motor2)), 255);
	int order3 = min(abs((int) (255. * motor3)), 255);
	int order4 = min(abs((int) (255. * motor4)), 255);
	Generate_order(1, order1, motor1 > 0);
	Generate_order(2, order2, motor2 > 0);
	Generate_order(3, order3, motor3 > 0);
	Generate_order(4, order4, motor4 > 0);
//	cout << endl;
}

void Motors::Generate_order(int num_motor, int power, bool positive){
//	cout << "Motor" << num_motor << " : " << (positive ? "+" : "-") << power << endl;
	#if defined ENABLE_SERIAL && defined ENABLE_MOTORS
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
		serial->Lock();
		i2c->I2C_write(order, 8);
		serial->Unlock();
	#endif
}

void Motors::Set_i2c(I2C* i2c){this->i2c = i2c;}
