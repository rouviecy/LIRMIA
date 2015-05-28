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
#include "./State_machine.h"

class Autonomy : public ComThread{

public:

	Autonomy();
	~Autonomy();

private:

	void On_start();
	void Job();
	void IO();

	int fsm_state;
	float remote_forward, remote_turn;
	float t;
	float xy[2];
	float thz;
	float cam_detect_horizontal;
	float cam_detect_vertical;

	float motor;
	float rudder;
	float bow_thruster;

};

#endif
