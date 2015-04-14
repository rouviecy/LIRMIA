/*
 * @(#)		State_machine.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * State machine of the robot
 */

#ifndef STATE_MACHINE
#define STATE_MACHINE

#include <cmath>
#include <iostream>
#include "../core/ComThread.h"
#include "../interfaces/FSM.h"
#include "../interfaces/FSMDraw.h"

class State_machine : public ComThread{

public:

	State_machine();
	~State_machine();

private:

	FSM fsm;
	FSMDraw drawer;

	bool my_guard;

	float z;
	float remote;
	float cam_detect1;
	float cam_detect2;
	float fsm_state;

	void On_start();
	void Job();
	void IO();


};

#endif
