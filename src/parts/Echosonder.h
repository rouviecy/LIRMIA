/*
 * @(#)		Echosonder.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Echosonder measures distance to obstacles with serial
 */

#ifndef ECHOSONDER
#define ECHOSONDER

#include "../core/ComThread.h"
#include "../interfaces/Serial.h"

class Echosonder : public ComThread{

public:

	Echosonder();
	~Echosonder();

	void Set_serial_x(Serial* serial);
	void Set_serial_y(Serial* serial);

private:

	Serial *serial_x, *serial_y;

	void On_start();
	void Job();
	void IO();

	float obst_xy[2];

	unsigned char request;

	float Measure_distance(Serial* serial);

};

#endif
