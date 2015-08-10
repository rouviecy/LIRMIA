/*
 * @(#)		Sayab.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Sayab roboat
 */

#ifndef SAYAB
#define SAYAB

#include "../core/Maestro.h"
#include "../interfaces/Serial.h"
#include "../parts/sayab/Acoustic_modem.h"
#include "../parts/sayab/Autonomy.h"
#include "../parts/sayab/Cameras.h"
#include "../parts/sayab/Logger.h"
#include "../parts/sayab/Motors.h"
#include "../parts/sayab/Remote_control.h"
#include "../parts/sayab/Remote_monitor.h"
#include "../parts/sayab/Simulator.h"
#include "../parts/sayab/State.h"
#include "../parts/sayab/State_machine.h"
#include "../parts/Clock.h"
#include "../parts/Compass_CMPS10.h"
#include "../parts/Gps.h"
#include "../parts/Subscriber.h"

#define DEV_SERIAL_ARDUINO	"/dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_64131383231351E06150-if00"
#define DEV_SERIAL_ISS		"/dev/serial/by-id/usb-Devantech_Ltd._USB-ISS._00007347-if00"

// TO CHECK :
//#define DEV_SERIAL_RS232	"/dev/serial/by-id/usb-Prolific_Technology_Inc._USB-Serial_Controller-if00-port0"
#define DEV_SERIAL_RS232	"/dev/ttyUSB0"

class Sayab : public Maestro{

public:

	Sayab();
	void Shutdown();

private:

	Serial serial_arduino, serial_iss, serial_rs232;

	void Init_serial();

	Acoustic_modem	acoustic_modem;
	Autonomy	autonomy;
	Cameras		cameras;
	Compass_CMPS10	compass;
	Clock		internal_clock;
	Gps		gps;
	Logger		logger;
	Motors		motors;
	Remote_control	remote_control;
	Remote_monitor	remote_monitor;
	Simulator	simulator;
	State		state;
	State_machine	state_machine;
	Subscriber	subscriber;

};

#endif
