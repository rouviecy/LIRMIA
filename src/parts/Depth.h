/*
 * @(#)		Depth.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Direct depth measurement with I2C
 */

#ifndef DEPTH
#define DEPTH

#include "../core/ComThread.h"
#include "../interfaces/Serial.h"

class Depth : public ComThread{

public:

	Depth();
	~Depth();

	void Calibrate();
	void Set_serial(Serial* serial);

private:

	Serial* serial;

	void On_start();
	void Job();
	void IO();

	float t;
	float depth;
	float z_init;
	int calib_params[6];

};

#endif
