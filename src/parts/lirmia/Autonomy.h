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
	float remote_forward, remote_turn, remote_deeper;
	float t;
	float xyz[3];
	float vxyz[3];
	float thxyz[3];
	float vthxyz[3];
	float obst_xy[2];
	float obst_vxy[2];
	float cam_size_obj[2];
	float cam_detect_horizontal[2];
	float cam_detect_vertical[2];
	float cam_pipeline_angle[2];
	float cam_pipeline_distance[2];
	float motor[4];

	float keep_thz;

};

#endif
