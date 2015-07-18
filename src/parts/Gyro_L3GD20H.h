/*
 * @(#)		Gyro_L3GD20H.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Gyro sensor : L3GD20H
 */

#ifndef L3GD20H
#define L3GD20H

#include "../core/ComThread.h"
#include "../interfaces/Serial.h"
#include "../interfaces/I2C.h"
#include <cmath>

#define L3GD20H_I2C_ADDRESS	0x53

class Gyro_L3GD20H : public ComThread{

public:

	Gyro_L3GD20H();
	~Gyro_L3GD20H();

	void Set_i2c(I2C* i2c);

private:

	I2C* i2c;

	void On_start();
	void Job();
	void IO();

	float gyro_vthxyz[3];

	unsigned char request_gyro_i2c[2];

};

#endif
