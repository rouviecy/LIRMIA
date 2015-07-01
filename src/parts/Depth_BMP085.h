/*
 * @(#)		Depth_BMP085.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Depth and temperature sensor : BMP085
 */

#ifndef BMP085
#define BMP085

#include "../core/ComThread.h"
#include "../interfaces/Serial.h"
#include "../interfaces/I2C.h"
#include <cmath>

#define BMP085_I2C_ADDRESS		0x77
#define BMP085_OVERSAMPLING_SETTING	3

class Depth_BMP085 : public ComThread{

public:

	Depth_BMP085();
	~Depth_BMP085();

	void Set_iss(Serial* serial);
	void Set_i2c(I2C* i2c);

private:

	Serial* serial;
	I2C* i2c;

	void On_start();
	void Job();
	void IO();

	float depth;

	short int ac1, ac2, ac3;
	unsigned short int ac4, ac5, ac6;
	short int b1, b2, mb, mc, md;

	int b5;
	unsigned int temperature, pressure, altitude;

	unsigned short Read_ushort_and_swap(int address);
	void Update_temperature();
	void Update_pressure();
	void Update_altitude();

};

#endif
