/*
 * @(#)		State.h
 * @version	1.0
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

	float compass_thxyz[3];
	float gyro_vthxyz[3];
	float xyz[3];
	float thxyz[3];
	float vthxyz[3];
	float depth;
	float t;

	void On_start();
	void Job();
	void IO();

};

#endif
