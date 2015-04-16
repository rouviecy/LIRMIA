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

#include "../core/ComThread.h"
#include "I2C.h"

class Imu : public ComThread{

public:

	Imu();
	~Imu();

	void Set_i2c(I2C* i2c);

private:

	#define FAC_ANG_IMU 0.0109863	// Factor de escala de los angulos YAW, PITCH y ROLL
	#define FAC_RAT_IMU 0.0137329	// Factor de escala de YAW RATE
	#define FAC_ACC_IMU 0.15387	// Factor de escala de ACELERACIÓN

	I2C* i2c;

	void On_start();
	void Job();
	void IO();

	float imu_thx,  imu_thy,  imu_thz;
	float imu_vthx, imu_vthy, imu_vthz;

	static void Generate_YPR(void* obj, std::string answer);

};

#endif
