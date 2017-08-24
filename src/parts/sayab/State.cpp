#include "State.h"

using namespace std;

State::State() : ComThread(){
//	last_t = -1;
	ti = 0.;
	tim = 0.;
	xy[0] = 0.; xy[1] = 0.; thz = 0.; vthz = 0.;
	lat = 0.; lon = 0., gpsreflat = 0., gpsreflon = 0., gpsbearing = 0.;
//	last_thz = 0;
	xm = 0.; xk = 0.; xk_1 = 0.; vk = 0.; vk_1 = 0.; rk = 0.;
	az = 0.09; bz = 0.3; dtz = 0.08;
	xd = 0.; xkd = 0.; xkd_1 = 0.; vkd = 0.; vkd_1 = 0.; rkd = 0.;
	ad = 0.09; bd = 0.3; dtd = 0.08; vd = 0.;
 	yawref = 0.; yawrefp = 0.;
	latmean = 0., dx = 0., dy = 0., distance = 0., bearing = 0.;
	ey1 = 0.; ey2 = 0.; ew = 0.; ewp = 0.; ef = 0.; efp = 0.;
	uw = 0.; uf = 0.;
	uwb = 0.; alfab1 = 1.5; alfab2 = 1; Iz = 1;
	uwpds = 0.; kpw = 0.; dpw = 5; bpw = 0.65; mupw = 1; kdw = 0.; ddw = 20; bdw = 0; mudw = 1;
	ufpd = 0.; kpf = 10; kdf = 1;
}

State::~State(){}

void State::On_start(){}

void State::IO(){
	Link_input("t",			COMFLOAT, 1, &t);
	Link_input("simu_xy",		COMFLOAT, 2, simu_xy);
	Link_input("simu_thz",		COMFLOAT, 1, &simu_thz);
	Link_input("compass",		COMFLOAT, 1, &compass);
	Link_input("ti",		COMFLOAT, 1, &ti);
	Link_input("gps_lat",		COMFLOAT, 1, &gps_lat);
	Link_input("gps_lon",		COMFLOAT, 1, &gps_lon);

	Link_output("xy",		COMFLOAT, 2, xy);
	Link_output("lat",		COMFLOAT, 1, &lat);
	Link_output("lon",		COMFLOAT, 1, &lon);
	Link_output("thz",		COMFLOAT, 1, &thz);
	Link_output("vthz",		COMFLOAT, 1, &vthz);
	Link_output("yawref",		COMFLOAT, 1, &yawref);
	Link_output("gpsreflat",	COMFLOAT, 1, &gpsreflat);
	Link_output("gpsreflon",	COMFLOAT, 1, &gpsreflon);
	Link_output("gpsbearing",	COMFLOAT, 1, &gpsbearing);
	Link_output("distance",		COMFLOAT, 1, &distance);
	Link_output("uw",		COMFLOAT, 1, &uw);
	Link_output("uf",		COMFLOAT, 1, &uf);
	Link_output("uwb", 		COMFLOAT, 1, &uwb);
	Link_output("uwpds",		COMFLOAT, 1, &uwpds);
	Link_output("ufpd",		COMFLOAT, 1, &ufpd);

	Link_output("alfab1",		COMFLOAT, 1, &alfab1);
	Link_output("alfab2",           COMFLOAT, 1, &alfab2);
	Link_output("kpw",              COMFLOAT, 1, &kpw);
        Link_output("kdw",              COMFLOAT, 1, &kdw);
	Link_output("dpw",              COMFLOAT, 1, &dpw);
        Link_output("bpw",              COMFLOAT, 1, &bpw);
        Link_output("mupw",             COMFLOAT, 1, &mupw);
        Link_output("ddw",              COMFLOAT, 1, &ddw);
        Link_output("bdw",              COMFLOAT, 1, &bdw);
        Link_output("mudw",             COMFLOAT, 1, &mudw);
	Link_output("kpf",		COMFLOAT, 1, &kpf);
	Link_output("kdf",		COMFLOAT, 1, &kdf);

	Link_output("Iz",               COMFLOAT, 1, &Iz);
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

	xd = distance;
	xkd = xkd_1 + (vkd_1*dtd);
	vkd = vkd_1;
	rkd = xd - xkd;
	xkd = xkd + ad*rkd;
	vkd = vkd + (bd*rkd)/dtd;
	xkd_1 = xkd;
	vkd_1 = 0.5 * vkd;
	vd = vkd_1;
	
//REFERENCES
	tim = t-ti;
	yawref = (50*sin(0.1*tim))+100; //sinus
	yawrefp = -5*cos(0.1*tim);
	//if(yawref > -180){yawref = 180 - 2.24375*tim;} //circle
	//else {yawref = -180;}
	//yawrefp = 2.24375;
	//yawrefp = gpsbearing;

	//gpsreflat = 0.340333; //(3)radian
	//gpsreflon = -1.73028; //(3)radian

	gpsreflat = 0.340332; //(4)radian
	gpsreflon = -1.73027; //(4)radian

//CALCULATE DISTANCE & BEARING GPS
latmean = (gpsreflat + lat)/2;

dx = R_EARTH * (gpsreflon - lon) * cos(latmean);
dy = R_EARTH * (gpsreflat - lat); 

distance = pow((dx*dx + dy*dy),0.5);

bearing = atan2 (sin(gpsreflon-lon)*cos(gpsreflat),(cos(lat)*sin(lat)*sin(gpsreflat)-sin(lat)*cos(gpsreflat)*cos(gpsreflon-lon)));
bearing = bearing*180/M_PI;
if(bearing<0){bearing+=360;}

gpsbearing = bearing;

// CONTROL LAWS

//Backstepping
	ey1 = yawref - thz;
	ey2 = vthz - alfab1*ey1 + yawrefp;
	uwb  = Iz*(-ey1 + alfab1*(ey2 + alfab1*ey1) + alfab2*ey2);

//Nonlinear PD based on Saturation Functions
	ew = yawref - thz;
	ewp = vthz;
        if(fabs(ew) > dpw) {kpw = bpw * pow(fabs(ew),(mupw - 1));}
        else              {kpw = bpw * pow(dpw,(mupw - 1));}
        if(fabs(ewp) > ddw){kdw = bdw * pow(fabs(ewp),(mudw - 1));}
        else              {kdw = bdw * pow(dpw,(mudw - 1));}
        uwpds = kpw * ew + kdw * ewp;

//Forward Control

	ef = distance;
	efp = vd;
	ufpd = kpf * ef + kdf * efp;
	

//Active Control
	uw = uwb;
//	uw = uwpds;
//	uf = ufpd;

	Critical_send();
}
