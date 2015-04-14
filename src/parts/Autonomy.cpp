#include "Autonomy.h"

using namespace std;

Autonomy::Autonomy() : ComThread(){
	motor1 = 0.;
	motor2 = 0.;
	motor3 = 0.;
	motor4 = 0.;
}

Autonomy::~Autonomy(){}

void Autonomy::On_start(){}

void Autonomy::IO(){
	Link_input("fsm_state", &fsm_state);
	Link_input("x", &x);        Link_input("y", &y);        Link_input("z", &z);
	Link_input("vx", &vx);      Link_input("vy", &vy);      Link_input("vz", &vz);
	Link_input("thx", &thx);    Link_input("thy", &thy);    Link_input("thz", &thz);
	Link_input("vthx", &vthx);  Link_input("vthy", &vthy);  Link_input("vthz", &vthz);
	Link_input("obst_x", &x);   Link_input("obst_y", &y);
	Link_input("obst_vx", &vx); Link_input("obst_vy", &vy);

	Link_output("motor1", &motor1);
	Link_output("motor2", &motor2);
	Link_output("motor3", &motor3);
	Link_output("motor4", &motor4);
}

void Autonomy::Job(){
	Critical_receive();

	Critical_send();
}
