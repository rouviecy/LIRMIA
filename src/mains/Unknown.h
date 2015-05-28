/*
 * @(#)		Unknown.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Unknown robot
 */

#ifndef UNKNOWN
#define UNKNOWN

#include "../core/Maestro.h"
#include "../interfaces/Serial.h"
#include "../parts/unknown/Autonomy.h"
#include "../parts/unknown/Cameras.h"
#include "../parts/unknown/Logger.h"
#include "../parts/unknown/Motors.h"
#include "../parts/unknown/Remote_control.h"
#include "../parts/unknown/Remote_monitor.h"
#include "../parts/unknown/Simulator.h"
#include "../parts/unknown/State.h"
#include "../parts/unknown/State_machine.h"
#include "../parts/Clock.h"
#include "../parts/Gps.h"
#include "../parts/I2C.h"

#define DEV_SERIAL_ARDUINO	"/dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_5533330393435131F151-if00"
#define DEV_SERIAL_ISS		"/dev/serial/by-id/usb-Devantech_Ltd._USB-ISS._00007350-if00"

class Unknown : public Maestro{

public:

	Unknown();
	void Shutdown();

private:

	Serial serial_arduino, serial_iss;

	void Init_serial();

	Autonomy	autonomy;
	Cameras		cameras;
	Clock		internal_clock;
	Gps		gps;
	I2C		i2c;
	Logger		logger;
	Motors		motors;
	Remote_control	remote_control;
	Remote_monitor	remote_monitor;
	Simulator	simulator;
	State		state;
	State_machine	state_machine;

};

#endif
