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

enum state_t {STAY, DOWN, EXPLORE, FOLLOW_OBJ_CAM, FOLLOW_PIPE_CAM, FOLLOW_WALL, UP, REMOTE};

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
	bool cam_detect_obj, cam_detect_pipe;
	bool fsm_unlocked, fsm_down, fsm_up, fsm_explore, fsm_nofollow, fsm_stabilize;

	void On_start();
	void Job();
	void IO();

	static void Update_fsm(void* obj);

};

#endif
