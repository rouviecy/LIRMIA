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

#define R_EARTH 6378137

class State : public ComThread{

public:

	State();
	~State();

private:

	float t;
	float ti;
	float tim;
	float simu_xy[2];
	float simu_thz;
	float gps_xy[2];
	float gps_lat;
	float gps_lon;
	float compass;

	float xy[2];
	float lat;
	float lon;
	float thz;
	float vthz;
	float yawref;
	float yawrefp;
	float gpsreflat;
	float gpsreflon;
	float gpsbearing;

	void On_start();
	void Job();
	void IO();

	float last_t;
	float last_thz;

	float xm,xk,vk,rk;
	float xk_1,vk_1;
	float az, bz, dtz;

	float xd,xkd,vkd,rkd;
	float xkd_1,vkd_1;
	float ad, bd, dtd;
	float vd;

	float latmean, dx, dy;
	float distance, bearing;

	float uw;
	float uf;
	float ew, ewp;
	float ef, efp;

	float ey1, ey2;
	float alfab1, alfab2;
	float Iz;
	float uwb;

	float dpw, bpw, mupw;
        float ddw, bdw, mudw;
        float kpw, kdw;
	float uwpds;

	float ufpd;
	float kpf, kdf;

};

#endif
