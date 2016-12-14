#include "State.h"

using namespace std;

State::State() : ComThread(){
	last_t = -1.;
	xyz[0] = 0.; vxyz[0] = 0.; thxyz[0] = 0.; vthxyz[0] = 0.; last_thxyz[0] = 0.;
	xyz[1] = 0.; vxyz[1] = 0.; thxyz[1] = 0.; vthxyz[1] = 0.; last_thxyz[1] = 0.;
	xyz[2] = 0.; vxyz[2] = 0.; thxyz[2] = 0.; vthxyz[2] = 0.; last_thxyz[2] = 0.; vz = 0.; vthz = 0.;
	Iz= 1;
	xm = 0.; xk = 0.; xk_1 = 0.; vk = 0.; vk_1 = 0.; rk = 0.; az = 0.09; bz = 0.3; dtz = 0.08;
	xmz=0.; xkz=0.; xkz_1=0.; vkz=0.; vkz_1=0.; rkz=0.; azz=0.09; bzz=0.3; dtzz=0.08;
	yawref = 0.; yawrefp = 0.; uw = 0.; uwaux = 0.; ew1 = 0.; ew2 = 0.; alfabw1 = 1; alfabw2 = 1;
	zref = 0.; zrefp = 0.; uz = 0.; uzaux = 0.; ez1 = 0.; ez2 = 0.; alfabz1 = 1; alfabz2 = 1;
	x1=0.; x2=0.; y1=0.; y2=0.; m=0.; y=0.;
}

State::~State(){}

void State::On_start(){}

void State::IO(){
	Link_input("t",			COMFLOAT, 1, &t);
	Link_input("ti",		COMFLOAT, 1, &ti);
	Link_input("depth",		COMFLOAT, 1, &depth);
	Link_input("imu_thxyz",		COMFLOAT, 3, imu_thxyz);
	Link_input("simu_xyz",		COMFLOAT, 3, simu_xyz);
	Link_input("simu_thxyz",	COMFLOAT, 3, simu_thxyz);
	Link_input("thzm",		COMFLOAT, 2, thzm);

	Link_output("xyz",		COMFLOAT, 3, xyz);
	Link_output("vxyz",		COMFLOAT, 3, vxyz);
	Link_output("thxyz",		COMFLOAT, 3, thxyz);
	Link_output("vthxyz",		COMFLOAT, 3, vthxyz);
	Link_output("yawref",		COMFLOAT, 1, &yawref);
	Link_output("zref",		COMFLOAT, 1, &zref);
	Link_output("ew1",		COMFLOAT, 1, &ew1);
	Link_output("ew2",		COMFLOAT, 1, &ew2);
	Link_output("uw",		COMFLOAT, 1, &uw);
	Link_output("uwaux",		COMFLOAT, 1, &uwaux);
	Link_output("alfabw1",		COMFLOAT, 1, &alfabw1);
	Link_output("alfabw2",		COMFLOAT, 1, &alfabw2);
	Link_output("ez1",		COMFLOAT, 1, &ez1);
	Link_output("ez2",		COMFLOAT, 1, &ez2);
	Link_output("uz",		COMFLOAT, 1, &uz);
	Link_output("uzaux", 		COMFLOAT, 1, &uzaux);
	Link_output("alfabz1",		COMFLOAT, 1, &alfabz1);
	Link_output("alfabz2",		COMFLOAT, 1, &alfabz2);
	Link_output("vthz",		COMFLOAT, 1, &vthz);
	Link_output("vz",		COMFLOAT, 1, &vz);
}

void State::Job(){
	Critical_receive();
	#ifdef ENABLE_SIMULATOR
		xyz[0] = simu_xyz[0];
		xyz[1] = simu_xyz[1];
		xyz[2] = simu_xyz[2];
		thxyz[0] = simu_thxyz[0];
		thxyz[1] = simu_thxyz[1];
		thxyz[2] = simu_thxyz[2];
	#else
		xyz[2] = depth;
		thxyz[0] = imu_thxyz[2];
		thxyz[1] = imu_thxyz[1];
		thxyz[2] = imu_thxyz[0];
		thzd[0]  = thzm[0];
		thzd[1]  = thzm[1];
 	#endif
//	if(t - last_t > 0.01){
//		for(int i = 0; i < 3; i++){
//			vthxyz[i] = (thxyz[i] - last_thxyz[i]) / (t - last_t);
//			last_thxyz[i] = thxyz[i];
//		}
//		last_t = t;
//	}

// ALPHA BETA FILTER
	xm = thxyz[2];
        xk = xk_1 + (vk_1*dtz);
        vk = vk_1;
        rk = xm - xk;
        xk = xk + az*rk;
        vk = vk + (bz*rk)/dtz;
        xk_1 = xk;
        vk_1 = 0.5 * vk;
        vthxyz[2] = vk_1;

	xmz = xyz[2];
        xkz = xkz_1 + (vkz_1*dtzz);
        vkz = vkz_1;
        rkz = xmz - xkz;
        xkz = xkz + azz*rkz;
        vkz = vkz + (bzz*rkz)/dtzz;
        xkz_1 = xkz;
        vkz_1 = 0.5 * vkz;
        vxyz[2] = vkz_1;

//REFERENCES
	tim=t-ti;
	yawref = (50*sin(0.1*tim))+100; //sinus
        yawrefp = -5*cos(0.1*tim);
        //if(yawref > -180){yawref = 180 - 2.24375*tim;} //circle
	//else {yawref = -180;}
        //yawrefp = 2.24375;
	//yawref = yawmod;

	if(0<=tim  && tim<70){		x1=0;    y1=0;     x2=60;    y2=4;}
	if(70<=tim && tim<100){		x1=70;   y1=4;     x2=90;    y2=2.5;}
	if(100<=tim && tim<130){	x1=100;	 y1=2.5;   x2=120;   y2=1;}
	if(130<=tim){			x1=130;  y1=1;     x2=150;   y2=0;}

	m=(y2-y1)/(x2-x1);
	y=m*(tim-x1) + y1;

	if( m>0 ) {if(y>y2){y=y2;}}
	else	  {if(y<y2){y=y2;}}
	zref  = y;
	zrefp = 0;

//CONTROL LAWS
	ew1 = yawref - thxyz[2];
	ew2 = vthxyz[2] - alfabw1*ew1 + yawrefp;
	uw  = Iz*(-ew1 + alfabw1*(ew2 + alfabw1*ew1) + alfabw2*ew2);
	uwaux = uw*0.005;

	ez1 = zref - xyz[2];
	ez2 = vxyz[2] - alfabz1*ez1 + zrefp;
	uz  = Iz*(-ez1 + alfabz1*(ez2 + alfabz1*ez1) + alfabz2*ez2);
	uzaux = uz*0.005;

	Critical_send();
}
