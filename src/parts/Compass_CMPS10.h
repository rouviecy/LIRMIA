/*
 * @(#)		Compass_CMPS10.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Compass sensor : CMPS10
 */

#ifndef CMPS10
#define CMPS10

#include "../core/ComThread.h"
#include "../interfaces/Serial.h"

class Compass_CMPS10 : public ComThread{

public:

	Compass_CMPS10();
	~Compass_CMPS10();

	void Set_iss(Serial* serial);

private:

	Serial* serial;

	void On_start();
	void Job();
	void IO();

	float compass;

	unsigned char request_compass[4];

};

#endif
