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

class State : public ComThread{

public:

	State();
	~State();

private:

	float t;
	float simu_xy[2];
	float simu_thz;
	float gps_xy[2];
	float compass;

	float xy[2];
	float thz;

	void On_start();
	void Job();
	void IO();

};

#endif
