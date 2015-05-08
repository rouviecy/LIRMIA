#include "State.h"

using namespace std;

State::State() : ComThread(){
	last_t = -1.;
	xyz[0] = 0.; vxyz[0] = 0.; thxyz[0] = 0.; vthxyz[0] = 0.; last_imu_thxyz[0] = 0.;
	xyz[1] = 0.; vxyz[1] = 0.; thxyz[1] = 0.; vthxyz[1] = 0.; last_imu_thxyz[1] = 0.;
	xyz[2] = 0.; vxyz[2] = 0.; thxyz[2] = 0.; vthxyz[2] = 0.; last_imu_thxyz[2] = 0.;
}

State::~State(){}

void State::On_start(){}

void State::IO(){
	Link_input("t",		COMFLOAT, 1, &t);
	Link_input("imu_thxyz",	COMFLOAT, 3, imu_thxyz);

	Link_output("xyz",	COMFLOAT, 3, xyz);
	Link_output("vxyz",	COMFLOAT, 3, vxyz);
	Link_output("thxyz",	COMFLOAT, 3, thxyz);
	Link_output("vthxyz",	COMFLOAT, 3, vthxyz);
}

void State::Job(){
	Critical_receive();
	thxyz[0] = imu_thxyz[2];
	thxyz[1] = imu_thxyz[1];
	thxyz[2] = imu_thxyz[0];
	if(t - last_t > 0.01){
		for(int i = 0; i < 3; i++){
			vthxyz[i] = (imu_thxyz[i] - last_imu_thxyz[i]) / (t - last_t);
			last_imu_thxyz[i] = imu_thxyz[i];
		}
		last_t = t;
	}
	Critical_send();
}
