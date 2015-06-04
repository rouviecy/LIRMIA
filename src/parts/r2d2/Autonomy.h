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
	float t;
	float cam_size_obj;
	float cam_detect_horizontal;
	float cam_detect_vertical;
	float cam_pipeline_angle;
	float cam_pipeline_distance;
	float motor[4];

	float keep_thz;

};

#endif
