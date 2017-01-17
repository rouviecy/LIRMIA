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
	float thzm[2];

	float xyz[3];
	float vxyz[3];
	float thxyz[3];
	float vthxyz[3];
	float vz;
	float vthz;
	float thzd[2];

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

};

#endif
