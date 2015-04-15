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
	Link_input("cam_detect1", &cam_detect1);
	Link_input("cam_detect2", &cam_detect2);
	Link_input("cam_detect1_horizontal", &cam_detect1_horizontal); Link_input("cam_detect1_vertical", &cam_detect1_vertical);
	Link_input("cam_detect2_horizontal", &cam_detect2_horizontal); Link_input("cam_detect2_vertical", &cam_detect2_vertical);

	Link_output("motor1", &motor1);
	Link_output("motor2", &motor2);
	Link_output("motor3", &motor3);
	Link_output("motor4", &motor4);
}

void Autonomy::Job(){
	Critical_receive();
	if(cam_detect1 > 0){
		motor1 = +cam_detect1_horizontal / 10;
		motor2 = -cam_detect1_horizontal / 10;
		motor3 = +cam_detect1_vertical / 10;
		motor4 = +cam_detect1_vertical / 10;
	}
	else{
		motor1 = 0.;
		motor2 = 0.;
		motor3 = 0.;
		motor4 = 0.;
	}
	Critical_send();
}
