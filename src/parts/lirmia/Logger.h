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
	float zref;
	float vthz, vz;
	float mydwxy[4];
	float uzpdc, uzb, uzpds;
	float kpcz, kdcz, kpw, kpz, kdw, kdz;
	float motor[4];
	float cam_pipeline_angle[2];
	float cam_pipeline_distance[2];
	float cam_detect_horizontal[2];
	float cam_detect_vertical[2];
	float alfabz1;
	float alfabz2;
	float masa, WB;
	float dpz, bpz, mupz, ddz, bdz, mudz;
	float Iz, gcz;
	float uzpf;
        float g1, k1, k2, k3, f1, h;

	std::ofstream log_file;
	float last_t_save;

};

#endif
