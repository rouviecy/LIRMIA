/*
 * @(#)		Imu.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Inertial Measurement Unit with serial
 */

#ifndef IMU
#define IMU

#include <cmath>
#include <vector>
#include <algorithm>
#include "../core/ComThread.h"
#include "../interfaces/Serial.h"

class Imu : public ComThread{

public:

	Imu();
	~Imu();

	void Set_serial(Serial* serial);

private:

	Serial* serial;

	std::vector <char> msg;
	int header_size;

	void On_start();
	void Job();
	void IO();

	float imu_thxyz[3];

};

#endif
