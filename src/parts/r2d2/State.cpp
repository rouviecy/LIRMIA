#include "State.h"

using namespace std;

State::State() : ComThread(){
	xyz[0] = 0.;
	xyz[1] = 0.;
	xyz[2] = 0.;
	thxyz[0] = 0.;
	thxyz[1] = 0.;
	thxyz[2] = 0.;
	vthxyz[0] = 0.;
	vthxyz[1] = 0.;
	vthxyz[2] = 0.;
}

State::~State(){}

void State::On_start(){}

void State::IO(){
	Link_input("compass_thxyz",	COMFLOAT, 3, compass_thxyz);
	Link_input("imu_vthz",		COMFLOAT, 1, &imu_vthz);
	Link_input("depth",		COMFLOAT, 1, &depth);
	Link_input("t",			COMFLOAT, 1, &t);

	Link_output("xyz",		COMFLOAT, 3, xyz);
	Link_output("thxyz",		COMFLOAT, 3, thxyz);
	Link_output("vthxyz",		COMFLOAT, 3, vthxyz);

}

void State::Job(){
	Critical_receive();
	thxyz[0] = compass_thxyz[0];
	thxyz[1] = compass_thxyz[1];
	thxyz[2] = compass_thxyz[2];
	vthxyz[2] = imu_vthz;
	xyz[2] = depth;
	Critical_send();
}
