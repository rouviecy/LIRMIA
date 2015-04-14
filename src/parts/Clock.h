/*
 * @(#)		Clock.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Track time
 */

#ifndef CLOCK
#define CLOCK

#include <chrono>
#include "../core/ComThread.h"

class Clock : public ComThread {

public:

	Clock();
	~Clock();

private:

	void On_start();
	void Job();
	void IO();

	float t;

	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point now;

};

#endif
