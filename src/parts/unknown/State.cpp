#include "State.h"

using namespace std;

State::State() : ComThread(){
	xy[0] = 0.;
	xy[1] = 0.;
	thz = 0.;
}

State::~State(){}

void State::On_start(){}

void State::IO(){
	Link_input("t",			COMFLOAT, 1, &t);
	Link_input("simu_xy",		COMFLOAT, 2, simu_xy);
	Link_input("simu_thz",		COMFLOAT, 1, &simu_thz);
	Link_input("gps_xy",		COMFLOAT, 2, gps_xy);
	Link_input("compass",		COMFLOAT, 1, &compass);

	Link_output("xy",		COMFLOAT, 2, xy);
	Link_output("thz",		COMFLOAT, 1, &thz);
}

void State::Job(){
	Critical_receive();
	#ifdef ENABLE_SIMULATOR
		xy[0] = simu_xy[0];
		xy[1] = simu_xy[1];
		thz = simu_thz;
	#else
		xy[0] = gps_xy[0];
		xy[1] = gps_xy[1];
		thz = compass;
	#endif
	Critical_send();
}
