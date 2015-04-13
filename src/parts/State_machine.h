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

	float data_in;

	void On_start();
	void Job();
	void IO();

	static void Action_1to2(void* obj);
	static void Action_2to3(void* obj);
	static void Action_3to1(void* obj);

};

#endif
