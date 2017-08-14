#include "State.h"

using namespace std;

State::State() : ComThread(){
	last_t = -1.;
	xyz[0] = 0.; vxyz[0] = 0.; thxyz[0] = 0.; vthxyz[0] = 0.; last_thxyz[0] = 0.;
	xyz[1] = 0.; vxyz[1] = 0.; thxyz[1] = 0.; vthxyz[1] = 0.; last_thxyz[1] = 0.;
	xyz[2] = 0.; vxyz[2] = 0.; thxyz[2] = 0.; vthxyz[2] = 0.; last_thxyz[2] = 0.; vz = 0.; vthz = 0.; vthy = 0.;
	thzd[0] = 0.; thzd[1] = 0.;
	xm = 0.; xk = 0.; xk_1 = 0.; vk = 0.; vk_1 = 0.; rk = 0.; az = 0.09; bz = 0.3; dtz = 0.08;
	xmp=0.; xkp=0.; xkp_1=0.; vkp=0.; vkp_1=0.; rkp=0.; azp=0.09; bzp=0.3; dtzp=0.08;
	xmz=0.; xkz=0.; xkz_1=0.; vkz=0.; vkz_1=0.; rkz=0.; azz=0.09; bzz=0.3; dtzz=0.08;
	x1=0.; x2=0.; y1=0.; y2=0.; m=0.; y=0.;
	yawref = 0.; yawrefp = 0.; zref = 0.; zrefp = 0.;
	ew = 0.; ewp = 0.; ez = 0.; ezp = 0.; ewb = 0.; ezb = 0.;
	uw = 0.; uz = 0.;

	uzpdc = 0.; kpcz =90; kdcz = 8.5; gcz = 30;

	uwb = 0.; alfabw1 = 0.1; alfabw2 = 0.01;
	uzb = 0.; alfabz1 = 1; alfabz2 = 1; Iz = 1;

	uwpds = 0.; kpw = 0.; dpw = 5; bpw = 0.65; mupw = 1; kdw = 0.; ddw = 20; bdw = 0; mudw = 1;
	uzpds = 0.; kpz = 0.; dpz = 5; bpz = 0.65; mupz = 1; kdz = 0.; ddz = 20; bdz = 0; mudz = 1;

	uzpf = 0.; uzpf_actual = 0.; uzpf_anterior = 0.; uzpf_2anterior = 0.;
	theta = 0.; q = 0.; z = 0.;
	g1 = 0; k1 = 0; k2 = 0; k3 = 0; f1 = 0;
	a11 = 0; a13 = 0; a23 = 0; b1 = 0; h = 0;
	g_kb = 0.; ka_gk1 = 0., ka_gk2 = 0.; ka_gk3 = 0.; gfref = 0.;
        xt_tauc = 0.; xt_tau1c = 0.; xt_tau2c = 0.; xt_tau3c = 0.;
        xt_tauv = 0.; xt_tau1v = 0.; xt_tau2v = 0.; xt_tau3v = 0.;

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
	Link_input("modmsg",		COMFLOAT, 3, modmsg);

	Link_output("xyz",		COMFLOAT, 3, xyz);
	Link_output("vxyz",		COMFLOAT, 3, vxyz);
	Link_output("thxyz",		COMFLOAT, 3, thxyz);
	Link_output("vthxyz",		COMFLOAT, 3, vthxyz);
	Link_output("yawref",		COMFLOAT, 1, &yawref);
	Link_output("zref",		COMFLOAT, 1, &zref);
	Link_output("vthz",             COMFLOAT, 1, &vthz);
        Link_output("vz",               COMFLOAT, 1, &vz);
        Link_output("mywxy",            COMFLOAT, 2, mywxy);

	Link_output("uw",		COMFLOAT, 1, &uw);
	Link_output("uz",		COMFLOAT, 1, &uz);
	Link_output("uzpdc",		COMFLOAT, 1, &uzpdc);
	Link_output("uwb",		COMFLOAT, 1, &uwb);
	Link_output("uzb",		COMFLOAT, 1, &uzb);
	Link_output("uwpds",		COMFLOAT, 1, &uwpds);
	Link_output("uzpds",		COMFLOAT, 1, &uzpds);
	Link_output("kpcz",		COMFLOAT, 1, &kpcz);
	Link_output("kdcz",		COMFLOAT, 1, &kdcz);
	Link_output("uzpf",		COMFLOAT, 1, &uzpf);


	Link_output("alfabw1",		COMFLOAT, 1, &alfabw1);
	Link_output("alfabw2",		COMFLOAT, 1, &alfabw2);
	Link_output("alfabz1",          COMFLOAT, 1, &alfabz1);
        Link_output("alfabz2",          COMFLOAT, 1, &alfabz2);
	Link_output("kpw",		COMFLOAT, 1, &kpw);
	Link_output("kpz",		COMFLOAT, 1, &kpz);
	Link_output("kdw",		COMFLOAT, 1, &kdw);
	Link_output("kdz",		COMFLOAT, 1, &kdz);
	Link_output("dpw",		COMFLOAT, 1, &dpw);
	Link_output("dpz", 		COMFLOAT, 1, &dpz);
	Link_output("bpw",		COMFLOAT, 1, &bpw);
	Link_output("bpz",		COMFLOAT, 1, &bpz);
	Link_output("mupw",		COMFLOAT, 1, &mupw);
	Link_output("mupz",		COMFLOAT, 1, &mupz);
	Link_output("ddw",             	COMFLOAT, 1, &ddw);
        Link_output("ddz",              COMFLOAT, 1, &ddz);
        Link_output("bdw",             	COMFLOAT, 1, &bdw);
	Link_output("bdz",		COMFLOAT, 1, &bdz);
	Link_output("mudw",		COMFLOAT, 1, &mudw);
	Link_output("mudz",		COMFLOAT, 1, &mudz);
	Link_output("g1",            	COMFLOAT, 1, &g1);
	Link_output("k1",	        COMFLOAT, 1, &k1);
	Link_output("k2",		COMFLOAT, 1, &k2);
	Link_output("k3", 	        COMFLOAT, 1, &k3);
	Link_output("f1",	        COMFLOAT, 1, &f1);
	Link_output("h",		COMFLOAT, 1, &h);

	Link_output("Iz",		COMFLOAT, 1, &Iz);
	Link_output("gcz",		COMFLOAT, 1, &gcz);
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
		mywxy[0] = modmsg[0];
		mywxy[1] = modmsg[1];
		mywxy[2] = modmsg[2];
 	#endif

//cout << "z : " <<  xyz[2] << endl;
//	if(t - last_t > 0.01){
//		for(int i = 0; i < 3; i++){
//			vthxyz[i] = (thxyz[i] - last_thxyz[i]) / (t - last_t);
//			last_thxyz[i] = thxyz[i];
//		}
//		last_t = t;
//	}

// ALPHA BETA FILTER
//cout << "yaw =" << thxyz[2] << endl;
	xm = thxyz[2];
        xk = xk_1 + (vk_1*dtz);
        vk = vk_1;
        rk = xm - xk;
        xk = xk + az*rk;
        vk = vk + (bz*rk)/dtz;
        xk_1 = xk;
        vk_1 = 0.5 * vk;
        vthxyz[2] = vk_1;
	vthz = vthxyz[2];

        xmp = thxyz[1];
        xkp = xkp_1 + (vkp_1*dtzp);
        vkp = vkp_1;
        rkp = xmp - xkp;
        xkp = xkp + azp*rkp;
        vkp = vkp + (bzp*rkp)/dtzp;
        xkp_1 = xkp;
        vkp_1 = 0.5 * vkp;
        vthxyz[1] = vkp_1;
        vthy = vthxyz[1];

	xmz = xyz[2];
        xkz = xkz_1 + (vkz_1*dtzz);
        vkz = vkz_1;
        rkz = xmz - xkz;
        xkz = xkz + azz*rkz;
        vkz = vkz + (bzz*rkz)/dtzz;
        xkz_1 = xkz;
        vkz_1 = 0.5 * vkz;
        vxyz[2] = vkz_1;
	vz = vxyz[2];

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
//	y=m*(tim-x1) + y1;
	//y = 0.5;

	if( m>0 ) {if(y>y2){y=y2;}}
	else	  {if(y<y2){y=y2;}}
	zref  = y;
	zrefp = 0;

//CONTROL LAWS

//PD controller with gravity/buoyancy compensation
	ez = zref - xyz[2];
	ezp = vxyz[2];
	uzpdc = kpcz * ez + kdcz * ezp + gcz;

//Backstepping
	ew = yawref - thxyz[2];
	ewb = vthxyz[2] - alfabw1*ew + yawrefp;
	uwb  = Iz*(-ew + alfabw1*(ewb + alfabw1*ew) + alfabw2*ewb);

	ezb = vxyz[2] - alfabz1*ez + zrefp;
	uzb = Iz*(-ez + alfabz1*(ezb + alfabz1*ezb) + alfabz2*ezb);

//Nonliner PD based on saturation functions
	ewp = vthxyz[2];
	if(fabs(ew) > dpw) {kpw = bpw * pow(fabs(ew),(mupw - 1));}
	else		  {kpw = bpw * pow(dpw,(mupw - 1));}
	if(fabs(ewp) > ddw){kdw = bdw * pow(fabs(ewp),(mudw - 1));}
	else		  {kdw = bdw * pow(dpw,(mudw - 1));}
	uwpds = kpw * ew + kdw * ewp;


	if(fabs(ez) > dpz) {kpz = bpz * pow(fabs(ez),(mupz - 1));}
        else		  {kpz = bpz * pow(dpz,(mupz - 1));}
        if(fabs(ezp) > ddz){kdz = bdz * pow(fabs(ezp),(mudz - 1));}
        else		  {kdz = bdz * pow(dpz,(mudz - 1));}
        uzpds = kpz * ez + kdz * ezp;

//PREDICTOR FILTER

	theta = thxyz[1];
        q = vthxyz[1];

        g_kb     = g1 + k1 * b1;
        ka_gk1   = (a11 * k1) + k3 - (g1*k1);
	xt_tau1c = (0.8376 * q) - (0.1462 * theta);
        ka_gk2   = - g1 * k2;
	xt_tau2c = (-0.0237 * q) + z - (0.2487 * theta);
        ka_gk3   = (a13 * k1)  + (a23 * k2) - (g1 * k3);
	xt_tau3c = (0.1838 * q) + (0.9849 * theta);
        gfref    = -g1 * f1 * zref;

	xt_tauc  = ka_gk1 * xt_tau1c + ka_gk2 * xt_tau2c + ka_gk3 * xt_tau3c;

	xt_tau1v = (1 - 0.8014 * h - 0.1534 *h*h*0.5 + 0.7605*h*h*h*0.16)*b1*uzpf_anterior + (1 - 0.8014 *2*h - 0.1534 *2*h*h + 0.7605*h*h*h*1.33)*b1*uzpf_2anterior;
	xt_tau2v = (-1.25*h*h*0.5 + 1.0017*h*h*h*0.16)*b1*uzpf_anterior + (-1.25*h*h*2 + 1.0017*h*h*h*1.33)*b1*uzpf_2anterior;
	xt_tau3v = (h - 0.8014*h*h*0.5 - 0.1534*h*h*h*0.16)*b1*uzpf_anterior + (2*h - 0.8014*h*h*2 - 0.1534*h*h*h*1.33)*b1*uzpf_2anterior;

	xt_tauv = ka_gk1 * xt_tau1v + ka_gk2 * xt_tau2v + ka_gk3 * xt_tau3v;

        uzpf_actual = (g_kb * uzpf_anterior) + gfref + xt_tauc + xt_tauv;

        uzpf = uzpf_actual + uzpf_anterior;

	uzpf_2anterior = uzpf_anterior;
        if (fabs(ez) < 2) {uzpf_anterior = 0;}
        else {uzpf_anterior = uzpf;}

//Active Control
	uw = uwb;
//	uw = uwpds;
//	uz = uzpdc;
//	uz = uzb;
//	uz = uzpds;
//	uz = uzpf
	Critical_send();
}
