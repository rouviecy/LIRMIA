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
#include "../core/ComThread.h"
#include "../interfaces/Serial.h"

class Imu : public ComThread{

public:

	Imu();
	~Imu();

	void Set_serial(Serial* serial);

private:

	Serial* serial;

	char header[5];
	bool is_header;
	bool valid_msg;
	int index_header;
	std::string msg;

	void On_start();
	void Job();
	void IO();

	float imu_thx,  imu_thy,  imu_thz;

};

#endif
