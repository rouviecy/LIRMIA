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

	bool tzer;
	int fsm_state;
	float remote_forward, remote_turn;
	float t, ti;
	float xy[2];
	float thz;
	float vthz;
	float yawref;
	float uw;
	float cam_detect_size[2];
	float cam_detect_horizontal[2];
	float cam_detect_vertical[2];
	//float uwaux;

	float motor;
	float rudder;
	float bow_thruster;

};

#endif
