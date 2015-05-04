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
#include "../core/ComThread.h"
#include "../interfaces/FSM.h"
#include "../interfaces/FSMDraw.h"

enum state_t {REMOTE, STAY, DOWN, EXPLORE, FOLLOW_OBJ_CAM1, FOLLOW_OBJ_CAM2, FOLLOW_PIPE_CAM1, FOLLOW_PIPE_CAM2, UP, UNKNOWN};

class State_machine : public ComThread{

public:

	State_machine();
	~State_machine();

	static state_t Decode_state(float float_state);
	static std::string Decode_state_str(float float_state);

private:

	FSM fsm;
	FSMDraw drawer;

	bool guard_unlocked;

	float z;
	float remote;
	float cam_detect_obj1, cam_detect_pipe1;
	float cam_detect_obj2, cam_detect_pipe2;
	float fsm_state, fsm_unlocked, fsm_down, fsm_up, fsm_explore, fsm_nofollow, fsm_stabilize;

	void On_start();
	void Job();
	void IO();

	state_t current_state;

	static void Act_to_stay(void* obj);
	static void Act_to_down(void* obj);
	static void Act_to_explore(void* obj);
	static void Act_to_follow_obj_cam1(void* obj);
	static void Act_to_follow_obj_cam2(void* obj);
	static void Act_to_follow_pipe_cam1(void* obj);
	static void Act_to_follow_pipe_cam2(void* obj);
	static void Act_to_up(void* obj);
	static void Act_to_remote(void* obj);

};

#endif
