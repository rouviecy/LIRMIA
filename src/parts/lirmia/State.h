/*
 * @(#)		State.h
 * @version	2.0
 * @autor	C. Rouvière
 */

/**
 * State of the robot
 */

#ifndef STATE
#define STATE

#include "../../core/ComThread.h"
#include "math.h"

class State : public ComThread{

public:

	State();
	~State();

private:

	float t;
	float ti;
	float tim;
	float depth;
	float imu_thxyz[3];
	float simu_xyz[3];
	float simu_thxyz[3];
	float modmsg[3];
	float mywxy[3];

	float xyz[3];
	float vxyz[3];
	float thxyz[3];
	float vthxyz[3];
	float vz;
	float vthz;
	float thzd[2];
	float vthy;

	void On_start();
	void Job();
	void IO();


	float last_t;
	float last_thxyz[3];

	float xm,xk,vk,rk;
	float xk_1,vk_1;
	float az,bz,dtz;

	float xmz,xkz,vkz,rkz;
        float xkz_1,vkz_1;
        float azz,bzz,dtzz;

	float xmp,xkp,vkp,rkp;
        float xkp_1,vkp_1;
        float azp,bzp,dtzp;

	float x1,x2,y1,y2,m,y;

	float yawref,yawrefp;
	float zref, zrefp;
	float ew, ewp, ewb;
	float ez, ezp, ezb;

	float uw, uz;

	float uzpdc, kpcz, kdcz, gcz;

	float alfabw1,alfabw2;
	float Iz;
	float uwb;
	float ez1,ez2;
	float alfabz1,alfabz2;
	float uzb;

	float dpw, bpw, mupw;
	float ddw, bdw, mudw;
	float kpw, kdw;
	float uwpds;
	float dpz, bpz, mupz;
	float ddz, bdz, mudz;
	float kpz, kdz;
	float uzpds;

	float uzpf, uzpf_actual, uzpf_anterior, uzpf_2anterior;
	float theta, q, z;
        float g1, k1, k2, k3, f1;
        float a11, a13, a23, b1;
	float h;
        float g_kb, ka_gk1, ka_gk2, ka_gk3, gfref;
	float xt_tauc, xt_tau1c, xt_tau2c, xt_tau3c;
	float xt_tauv, xt_tau1v, xt_tau2v, xt_tau3v;

};

#endif
