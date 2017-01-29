/*
 * @(#)		Depth.h
 * @version	1.0
 * @autor	E. Antonio
 */

/**
 * Direct depth measurement with I2C
 */

#ifndef DEPTH_MS5803_H
#define DEPTH_MS5803_H

#include "../core/ComThread.h"
#include <algorithm>
#include "../interfaces/Serial.h"
#include "../interfaces/I2C.h"
#include <cmath>


#define DEPTH_MS5803_ADDRESS     0X76


class Depth_MS5803 : public ComThread{

public:

	Depth_MS5803();
	~Depth_MS5803();

	void Set_iss(Serial* serial);
	
	void Calibrate();

private:

	Serial* serial;

	void On_start();
	void Job();
	void IO();

	bool raz_depth;
	float t;
	float depth;
	unsigned long d1,d2;
	float off,dt,tem,P,sens,t2,off2,sens2,z,zini,tempe,Pres,zant;
	float Ca[10];
	float buf[8];
	unsigned char request_depth_iss_w[4];
        unsigned char request_depth_iss_r[3];
	unsigned char request_depth_iss_cr[3];

	static void Process_serial_data(void* object, char* input_msg);

};

#endif
