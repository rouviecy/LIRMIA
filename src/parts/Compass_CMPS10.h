/*
 * @(#)		Compass_CMPS10.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Compass sensor : CMPS10
 */

#ifndef CMPS10
#define CMPS10

#include "../core/ComThread.h"
#include "../interfaces/Serial.h"
#include "../interfaces/I2C.h"
#include <cmath>

class Compass_CMPS10 : public ComThread{

public:

	Compass_CMPS10();
	~Compass_CMPS10();

	void Set_iss(Serial* serial);
	void Set_i2c(I2C* i2c);

private:

	Serial* serial;
	I2C* i2c;

	void On_start();
	void Job();
	void IO();

	float compass;
	float compass_thxyz[3];

	bool enable_iss, enable_i2c;
	unsigned char request_compass_iss[4];
	unsigned char request_compass_i2c[1];

};

#endif
