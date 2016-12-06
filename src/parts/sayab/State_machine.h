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

//enum state_t {STAY, EXPLORE, REMOTE, LAW_CONTROL, FOLLOW_CAM_SUB, FOLLOW_CAM_SURF, LOST_CAM_SUB, LOST_CAM_SURF};
enum state_t {STAY, EXPLORE, REMOTE, LAW_CONTROL, FOLLOW_CAM_SUB, FOLLOW_CAM_SURF};
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
	bool fsm_law_control;
	bool cam_detect_obj[2];
	bool sub_is_underwater;
	bool fsm_unlocked, fsm_explore, fsm_nofollow, fsm_stabilize;

	void On_start();
	void Job();
	void IO();

	static void Update_fsm(void* obj);

};

#endif
