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

#include "../../core/ComThread.h"
#include "../../interfaces/Serial.h"
#include "../I2C.h"

class Motors : public ComThread{

public:

	Motors();
	~Motors();

	void Set_i2c(I2C* i2c);
	void Set_arduino(Serial* serial);

private:

	I2C* i2c;
	Serial* arduino;

	void On_start();
	void Job();
	void IO();

	void Generate_order_i2c(int num_motor, int power, bool positive);
	void Generate_order_arduino(int angle);

	float motor;
	float rudder;
	float bow_thrusters[2];

};

#endif
