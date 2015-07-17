/*
 * @(#)		Autonomy.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Take decisions
 */

#ifndef AUTONOMY
#define AUTONOMY

#include "../../core/ComThread.h"
#include "State_machine.h"

class Autonomy : public ComThread{

public:

	Autonomy();
	~Autonomy();

private:

	void On_start();
	void Job();
	void IO();

	int fsm_state;
	float remote_forward, remote_turn, remote_deeper;
	float cam_size_obj;
	float cam_detect_horizontal;
	float cam_detect_vertical;
	float t;
	float thxyz[3];
	float motor[4];

	float keep_thz;
	float last_cam_detect_horizontal, last_cam_detect_vertical, last_t;

};

#endif
