/*
 * @(#)		Imu.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * A sensor providing data
 */

#ifndef IMU
#define IMU

#include "../core/ComThread.h"

class Imu : public ComThread{

public:

	Imu();
	~Imu();

private:

	void On_start();
	void Job();
	void IO();

	float imu_thx,  imu_thy,  imu_thz;
	float imu_vthx, imu_vthy, imu_vthz;

};

#endif
