/*
 * @(#)		Motors.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Control motors with I2C
 */

#ifndef MOTORS
#define MOTORS

#include "../core/ComThread.h"
#include "../interfaces/Serial.h"

class Motors : public ComThread{

public:

	Motors();
	~Motors();

	void Set_serial(Serial* serial);

private:

	Serial* serial;

	void On_start();
	void Job();
	void IO();

	void Generate_order(int num_motor, int power, bool positive);

	float motor1;
	float motor2;
	float motor3;
	float motor4;

};

#endif
