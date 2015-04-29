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

#include "../core/ComThread.h"

class State : public ComThread{

public:

	State();
	~State();

private:

	float t;
	float imu_thx, imu_thy, imu_thz;

	float x,    y,    z;
	float vx,   vy,   vz;
	float thx,  thy,  thz;
	float vthx, vthy, vthz;

	void On_start();
	void Job();
	void IO();

	float last_t;
	float last_imu_thx, last_imu_thy, last_imu_thz;

};

#endif
