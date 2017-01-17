/*
 * @(#)		Logger.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Write logs on hard-drive
 */

#ifndef LOGGUER
#define LOGGUER

#include "../../core/ComThread.h"
#include "./State_machine.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <fstream>

class Logger : public ComThread{

public:

	Logger();
	~Logger();

private:

	void On_start();
	void Job();
	void IO();

	int fsm_state;
	float t;
	float xyz[3];
	float vxyz[3];
	float thxyz[3];
	float vthxyz[3];
	float yawref;
	float zref;
	float vthz, vz;
	float thzd[2];
	float uzpdc, uwb, uzb, uwpds, uzpds;
	float kpcz, kdcz, kpw, kpz, kdw, kdz;
	float motor[4];
	float cam_pipeline_angle[2];
	float cam_pipeline_distance[2];
	float cam_detect_horizontal[2];
	float cam_detect_vertical[2];
	float alfabw1;
	float alfabw2;
	float alfabz1;
	float alfabz2;
	float dpw, dpz, bpw, bpz, mupw, mupz, ddw, ddz, bdw, bdz, mudw, mudz;
	float Iz, gcz;


	std::ofstream log_file;
	float last_t_save;

};

#endif
