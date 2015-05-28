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
#include "../../core/ComThread.h"
#include "../../interfaces/FSM.h"
#include "../../interfaces/FSMDraw.h"

enum state_t {REMOTE, FOLLOW_OBJ_CAM, LOST_OBJ_CAM};

class State_machine : public ComThread{

public:

	State_machine();
	~State_machine();

	static std::string Decode_state_str(int int_state);

private:

	FSM fsm;
	FSMDraw drawer;

	int fsm_state;
	bool remote;
	bool cam_detect_obj;

	void On_start();
	void Job();
	void IO();

};

#endif
