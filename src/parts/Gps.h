/*
 * @(#)		Gps.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Gps
 */

#ifndef GPS
#define GPS

#include "../core/ComThread.h"
#include "../interfaces/Serial.h"

class Gps : public ComThread{

public:

	Gps();
	~Gps();

	void Set_serial(Serial* serial);

private:

	Serial *serial;

	void On_start();
	void Job();
	void IO();

	float gps_xy[2];

};

#endif
