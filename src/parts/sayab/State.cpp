#include "State.h"

using namespace std;

State::State() : ComThread(){
//	last_t = -1;
	ti = 0.;
	tim = 0.;
	xy[0] = 0.; xy[1] = 0.; thz = 0.; vthz = 0.;
	lat = 0.; lon = 0.;
//	last_thz = 0;
	xm = 0.; xk = 0.; xk_1 = 0.; vk = 0.; vk_1 = 0.; rk = 0.;
	az = 0.09; bz = 0.3; dtz = 0.08;
 	yawref = 0.; yawrefp = 0.;
	ey1 = 0.; ey2 = 0.; alfab1 = 1.2; alfab2 = 1;
	Iz = 1;
	uw = 0.; uwaux = 0.;
}

State::~State(){}

void State::On_start(){}

void State::IO(){
	Link_input("t",			COMFLOAT, 1, &t);
	Link_input("simu_xy",		COMFLOAT, 2, simu_xy);
	Link_input("simu_thz",		COMFLOAT, 1, &simu_thz);
	Link_input("compass",		COMFLOAT, 1, &compass);
	Link_input("ti",		COMFLOAT, 1, &ti);
//	Link_input("gps_lat",		COMFLOAT, 1, &gps_lat);
//	Link_input("gps_lon",		COMFLOAT, 1, &gps_lon);

	Link_output("xy",		COMFLOAT, 2, xy);
	Link_output("lat",		COMFLOAT, 1, &lat);
	Link_output("lon",		COMFLOAT, 1, &lon);
	Link_output("thz",		COMFLOAT, 1, &thz);
	Link_output("vthz",		COMFLOAT, 1, &vthz);
	Link_output("yawref",		COMFLOAT, 1, &yawref);
	Link_output("ey1",		COMFLOAT, 1, &ey1);
	Link_output("ey2", 		COMFLOAT, 1, &ey2);
	Link_output("uw",		COMFLOAT, 1, &uw);
	Link_output("uwaux",		COMFLOAT, 1, &uwaux);
	Link_output("tim",		COMFLOAT, 1, &tim);
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
		lat = gps_lat;
		lon = gps_lon;
		thz = compass;
	#endif
//	if(t - last_t > 0.01){
//                         vthz = (thz - last_thz) / (t - last_t);
//                        last_thz = thz;
//                        last_t = t;
//                }
// ALPHA BETA FILTER
	xm = thz;
	xk = xk_1 + (vk_1*dtz);
	vk = vk_1;
	rk = xm - xk;
	xk = xk + az*rk;
	vk = vk + (bz*rk)/dtz;
	xk_1 = xk;
	vk_1 = 0.5 * vk;
	vthz = vk_1;
//REFERENCES
	tim = t-ti;
	yawref = (50*sin(0.1*tim))+100; //sinus
	yawrefp = -5*cos(0.1*tim);
	//if(yawref > -180){yawref = 180 - 2.24375*tim;} //circle
	//else {yawref = -180;}
	//yawrefp = 2.24375;

// CONTROL LAWS
	ey1 = yawref - thz;
	ey2 = vthz - alfab1*ey1 + yawrefp;
	uw  = Iz*(-ey1 + alfab1*(ey2 + alfab1*ey1) + alfab2*ey2);
	uwaux = uw*0.005;

	Critical_send();
}
