/*
 * @(#)		Gyro_ITG3200.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Gyro sensor : L3GD20H
 */

#ifndef ITG3200
#define ITG3200

#include "../core/ComThread.h"
#include "../interfaces/Serial.h"
#include "../interfaces/I2C.h"
#include <cmath>

#define ITG3200_I2C_ADDRESS	0xD2

class Gyro_ITG3200 : public ComThread{

public:

	Gyro_ITG3200();
	~Gyro_ITG3200();

	void Set_i2c(I2C* i2c);

private:

	I2C* i2c;

	void On_start();
	void Job();
	void IO();

	float gyro_vthxyz[3];

	unsigned char request_gyro_z_high, request_gyro_z_low;

};

#endif
