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

enum state_t {REMOTE, STAY, DOWN, EXPLORE, FOLLOW_CAM1, FOLLOW_CAM2, UP, UNKNOWN};

class State_machine : public ComThread{

public:

	State_machine();
	~State_machine();

	static state_t Decode_state(float float_state);

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

	state_t current_state;

	static void Act_remote_to_stay(void* obj);
	static void Act_stay_to_down(void* obj);
	static void Act_down_to_explore(void* obj);
	static void Act_explore_to_follow_cam1(void* obj);
	static void Act_explore_to_follow_cam2(void* obj);
	static void Act_follow_to_explore(void* obj);
	static void Act_explore_to_up(void* obj);
	static void Act_up_to_stay(void* obj);
	static void Act_stay_to_remote(void* obj);

};

#endif
