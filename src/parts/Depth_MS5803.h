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
#include "./Subscriber.h"
#include <algorithm>

class Depth_MS5803 : public ComThread{

public:

	Depth_MS5803();
	~Depth_MS5803();

	void Subscribe(Subscriber* subscriber);

private:

	void On_start();
	void Job();
	void IO();

	float t;
	float depth;

	static void Process_serial_data(void* object, char* input_msg);

};

#endif
