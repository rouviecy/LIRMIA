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
#include "../parts/lirmia/Acoustic_modem.h"
#include "../parts/lirmia/Autonomy.h"
#include "../parts/lirmia/Cameras.h"
#include "../parts/lirmia/Logger.h"
#include "../parts/lirmia/Mapping.h"
#include "../parts/lirmia/Motors.h"
#include "../parts/lirmia/Remote_control.h"
#include "../parts/lirmia/Remote_monitor.h"
#include "../parts/lirmia/Simulator.h"
#include "../parts/lirmia/State.h"
#include "../parts/lirmia/State_machine.h"
#include "../parts/Clock.h"
#include "../parts/Depth.h"
#include "../parts/Echosonder.h"
#include "../parts/Imu_razor.h"
#include "../parts/Subscriber.h"


#define DEV_SERIAL_ARDUINO	"/dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_5533330393435131F151-if00"
#define DEV_SERIAL_ISS		"/dev/serial/by-id/usb-Devantech_Ltd._USB-ISS._00007350-if00"
#define DEV_SERIAL_RS232	"/dev/serial/by-id/usb-Prolific_Technology_Inc._USB-Serial_Controller-if00-port0"

class Lirmia : public Maestro{

public:

	Lirmia();
	void Shutdown();

private:

	Serial serial_arduino, serial_iss, serial_rs232;

	void Init_serial();

	Acoustic_modem	acoustic_modem;
	Autonomy	autonomy;
	Cameras		cameras;
	Clock		internal_clock;
	Depth		depth;
	Echosonder	echosonder;
	Imu_razor	imu;
	Logger		logger;
	Mapping		mapping;
	Motors		motors;
	Remote_control	remote_control;
	Remote_monitor	remote_monitor;
	Simulator	simulator;
	State		state;
	State_machine	state_machine;
	Subscriber	subscriber;

};

#endif
