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
#include "I2C.h"

class Depth : public ComThread{

public:

	Depth();
	~Depth();

	void Calibrate();
	void Set_i2c(I2C* i2c);

private:

	I2C* i2c;

	void On_start();
	void Job();
	void IO();

	float t;
	float depth;
	float z_init;
	int calib_params[6];

};

#endif
