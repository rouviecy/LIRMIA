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
#include "../interfaces/Serial.h"

class Imu : public ComThread{

public:

	Imu();
	~Imu();

	void Set_serial(Serial* serial);

private:

	#define FAC_ANG_IMU 0.0109863	// Factor de escala de los angulos YAW, PITCH y ROLL
	#define FAC_RAT_IMU 0.0137329	// Factor de escala de YAW RATE
	#define FAC_ACC_IMU 0.15387	// Factor de escala de ACELERACIÓN

	Serial* serial;

	void On_start();
	void Job();
	void IO();

	float imu_thx,  imu_thy,  imu_thz;
	float imu_vthx, imu_vthy, imu_vthz;

};

#endif
