/*
 * @(#)		Lirmia.h
 * @version	2.0
 * @autor	C. Rouvière
 */

/**
 * LIRMIA
 */

#ifndef LIRMIA
#define LIRMIA

#include "../core/Maestro.h"
#include "../parts/Sensor.h"
#include "../parts/State.h"
#include "../parts/State_machine.h"

class Lirmia : public Maestro{

public:

	Lirmia();
	void Shutdown();

private:

	Sensor sensor;
	State state;
	State_machine state_machine;

};

#endif
