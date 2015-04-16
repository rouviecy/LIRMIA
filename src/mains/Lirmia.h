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
#include "../interfaces/Serial.h"
#include "../parts/Acoustic_modem.h"
#include "../parts/Autonomy.h"
#include "../parts/Clock.h"
#include "../parts/Cameras.h"
#include "../parts/Depth.h"
#include "../parts/Echosonder.h"
#include "../parts/I2C.h"
#include "../parts/Imu.h"
#include "../parts/Logger.h"
#include "../parts/Mapping.h"
#include "../parts/Motors.h"
#include "../parts/Remote.h"
#include "../parts/State.h"
#include "../parts/State_machine.h"

#define DEV_SERIAL "/dev/ttyACM0"

class Lirmia : public Maestro{

public:

	Lirmia();
	void Shutdown();

private:

	Serial serial;

	void Init_serial();

	Acoustic_modem	acoustic_modem;
	Autonomy	autonomy;
	Cameras		cameras;
	Clock		internal_clock;
	Depth		depth;
	Echosonder	echosonder;
	I2C		i2c;
	Imu		imu;
	Logger		logger;
	Mapping		mapping;
	Motors		motors;
	Remote		remote;
	State		state;
	State_machine	state_machine;

};

#endif
