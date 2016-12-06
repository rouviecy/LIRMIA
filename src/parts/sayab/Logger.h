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
	float tim;
	float xy[2];
	float gps_xy[2];
	float gps_lat;
	float gps_lon;
	float thz;
	float vthz;
	float yawref;
	float uw;
	float motor;
	float rudder;
	float bow_thruster;
	float ey1;
	float ey2;
	float alfab1;
	float alfab2;

//	float msgmod;

	std::ofstream log_file;
	float last_t_save;

};

#endif
