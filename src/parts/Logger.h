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

#include "../core/ComThread.h"

class Logger : public ComThread{

public:

	Logger();
	~Logger();

private:

	void On_start();
	void Job();
	void IO();

	float t;
	float fsm_state;
	float x,    y,    z;
	float vx,   vy,   vz;
	float thx,  thy,  thz;
	float vthx, vthy, vthz;

};

#endif
