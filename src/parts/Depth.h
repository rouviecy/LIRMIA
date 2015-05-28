/*
 * @(#)		Depth.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Direct depth measurement with I2C
 */

#ifndef DEPTH
#define DEPTH

#include "../core/ComThread.h"
#include "../interfaces/Serial.h"

class Depth : public ComThread{

public:

	Depth();
	~Depth();

	void Set_serial(Serial* serial);

private:

	Serial* serial;

	void On_start();
	void Job();
	void IO();

	std::vector <char> msg;
	int header_size;

	float t;
	float depth;

};

#endif
