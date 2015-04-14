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

class Motors : public ComThread{

public:

	Motors();
	~Motors();

private:

	void On_start();
	void Job();
	void IO();
	
	float motor1;
	float motor2;
	float motor3;
	float motor4;

};

#endif
