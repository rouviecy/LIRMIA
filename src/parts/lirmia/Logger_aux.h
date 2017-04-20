/*
 * @(#)		Logger_aux.h
 * @version	1.0
 * @autor	E. Antonio
 */

/**
 * Write logs on hard-drive
 */

#ifndef LOGGUER_AUX
#define LOGGUER_AUX

#include "../../core/ComThread.h"
#include "./State_machine.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <fstream>

class Logger_aux : public ComThread{

public:

	Logger_aux();
	~Logger_aux();

private:

	void On_start();
	void Job();
	void IO();

	int fsm_state;
	float t;
	float thxyz[3];
	float vthxyz[3];
	float yawref;
	float uwb, uwpds;
	float kpw, kdw;
	float motor[4];
	float cam_pipeline_angle[2];
	float cam_pipeline_distance[2];
	float cam_detect_horizontal[2];
	float cam_detect_vertical[2];
	float alfabw1;
	float alfabw2;
	float dpw, bpw, mupw, ddw, bdw, mudw;
	float Iz;

	std::ofstream log_file;
	float last_t_save;

};

#endif
