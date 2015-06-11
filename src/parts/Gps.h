/*
 * @(#)		Gps.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Gps sensor : EM-506
 */

#ifndef GPS
#define GPS

#include "../core/ComThread.h"
#include "./Subscriber.h"
#include <algorithm>

#define R_EARTH 6378137.

class Gps : public ComThread{

public:

	Gps();
	~Gps();

	void Subscribe(Subscriber* subscriber);

private:

	void On_start();
	void Job();
	void IO();

	float gps_xy[2];

	bool first;
	float offset_x, offset_y;

	static void Process_serial_data(void* object, char* input_msg);

};

#endif
