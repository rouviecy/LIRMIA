/*
 * @(#)		Imu_razor.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Inertial Measurement Unit with serial
 */

#ifndef IMU_RAZOR
#define IMU_RAZOR

#include <cmath>
#include <vector>
#include <algorithm>
#include "../core/ComThread.h"
#include "../interfaces/Serial.h"

class Imu_razor : public ComThread{

public:

	Imu_razor();
	~Imu_razor();

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
