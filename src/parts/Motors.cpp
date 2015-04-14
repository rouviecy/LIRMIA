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
}
