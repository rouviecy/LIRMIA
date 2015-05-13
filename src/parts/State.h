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
	float imu_thxyz[3];
	float simu_xyz[3];
	float simu_thxyz[3];

	float xyz[3];
	float vxyz[3];
	float thxyz[3];
	float vthxyz[3];

	void On_start();
	void Job();
	void IO();

	float last_t;
	float last_thxyz[3];

};

#endif
