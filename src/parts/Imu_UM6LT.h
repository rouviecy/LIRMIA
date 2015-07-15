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
#include "../interfaces/I2C.h"

#define UM6LT_I2C_ADDRESS	0x60

class Imu_UM6LT : public ComThread{

public:

	Imu_UM6LT();
	~Imu_UM6LT();

	void Set_i2c(I2C* i2c);

private:

	I2C* i2c;

	void On_start();
	void Job();
	void IO();

	float imu_thxyz[3];

};

#endif
