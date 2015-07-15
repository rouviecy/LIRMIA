/*
 * @(#)		Imu_UM6LT.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Inertial Measurement Unit with serial
 */

#ifndef IMU_UM6LT
#define IMU_UM6LT

#include <cmath>
#include <vector>
#include <algorithm>
#include "../core/ComThread.h"
#include "../interfaces/SPI.h"

class Imu_UM6LT : public ComThread{

public:

	Imu_UM6LT();
	~Imu_UM6LT();

	void Set_spi(SPI* spi);

private:

	SPI* spi;

	void On_start();
	void Job();
	void IO();

	float imu_thxyz[3];

};

#endif
