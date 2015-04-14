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
#include "../parts/Acoustic_modem.h"
#include "../parts/Autonomy.h"
#include "../parts/Clock.h"
#include "../parts/Cameras.h"
#include "../parts/Depth.h"
#include "../parts/Echosonder.h"
#include "../parts/Imu.h"
#include "../parts/Logger.h"
#include "../parts/Mapping.h"
#include "../parts/Motors.h"
#include "../parts/Remote.h"
#include "../parts/State.h"
#include "../parts/State_machine.h"

class Lirmia : public Maestro{

public:

	Lirmia();
	void Shutdown();

private:

	Acoustic_modem	acoustic_modem;
	Autonomy	autonomy;
	Cameras		cameras;
	Clock		internal_clock;
	Depth		depth;
	Echosonder	echosonder;
	Imu		imu;
	Logger		logger;
	Mapping		mapping;
	Motors		motors;
	Remote		remote;
	State		state;
	State_machine	state_machine;

};

#endif
