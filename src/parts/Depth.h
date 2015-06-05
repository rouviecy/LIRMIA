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

class Depth : public ComThread{

public:

	Depth();
	~Depth();

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
