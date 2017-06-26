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
	float tim;
	float xy[2];
	float gps_xy[2];
	float gps_lat;
	float gps_lon;
	float thz;
	float vthz;
	float gpsbearing;
	float gpsreflat;
	float gpsreflon;
	float distance;
	float uf;
	float ufpd;
	float motor;
	float rudder;
	float bow_thruster;
	float kpf, kdf;

//	float msgmod;

	std::ofstream log_file;
	float last_t_save;

};

#endif
