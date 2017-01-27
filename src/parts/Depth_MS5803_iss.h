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
#include <algorithm>
#include "../interfaces/Serial.h"
#include "../interfaces/I2C.h"
#include <cmath>


define DEPTH_MS5803_ISS_ADDRESS     0X76


class Depth_MS5803_iss : public ComThread{

public:

	Depth_MS5803_iss();
	~Depth_MS5803_iss();

	void Set_iss(Serial* serial);
	void Set_i2c(I2C* i2c);

private:

	Serial* serial
	I2C* i2c;

	void On_start();
	void Job();
	void IO();

	float t;
	float depth;
	unsigned long  d1,d2;
	float off,dt,tem,P,sens,t2,off2,sens2,z,zini,tempe,Pres,zant;
	float Ca[10];
	float buf[8];
	unsigned char request_compass_iss_w[4];
        unsigned char request_compass_iss_r[3];
	unsigned char request_compass_iss_cr[3];
        unsigned char request_compass_i2c[1];

	static void Process_serial_data(void* object, char* input_msg);

};

#endif
