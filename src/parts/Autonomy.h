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

#include <iostream>
#include "../core/ComThread.h"

class Autonomy : public ComThread{

public:

	Autonomy();
	~Autonomy();

private:

	void On_start();
	void Job();
	void IO();

	float fsm_state;
	float x,    y,    z;
	float vx,   vy,   vz;
	float thx,  thy,  thz;
	float vthx, vthy, vthz;
	float obst_x,  obst_y;
	float obst_vx, obst_vy;
	float cam_detect1;
	float cam_detect2;
	float cam_detect1_horizontal, cam_detect1_vertical;
	float cam_detect2_horizontal, cam_detect2_vertical;
	float motor1, motor2, motor3, motor4;

};

#endif
