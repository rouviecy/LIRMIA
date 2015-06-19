/*
 * @(#)		Simulator.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Generate fake data for sensor from motor orders
 */

#ifndef SIMULATOR
#define SIMULATOR

#include "../../core/ComThread.h"
#include <cmath>

class Simulator : public ComThread{

public:

	Simulator();
	~Simulator();

private:

	void On_start();
	void Job();
	void IO();

	float t;
	float simu_xy[2];
	float simu_thz;
	float motor;
	float rudder;
	float bow_thruster;

	float last_t;
	float acceleration[3];
	float velocity[3];

	void Saturate(float* number, float maximum, bool cycle);

};

#endif
