/*
 * @(#)		R2D2.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * LIRMIA
 */

#ifndef R2D2_H
#define R2D2_H

#include "../core/Maestro.h"
#include "../interfaces/Serial.h"
#include "../parts/r2d2/Autonomy.h"
#include "../parts/r2d2/Cameras.h"
#include "../parts/r2d2/Logger.h"
#include "../parts/r2d2/Motors.h"
#include "../parts/r2d2/Remote_control.h"
#include "../parts/r2d2/Remote_monitor.h"
#include "../parts/r2d2/State_machine.h"
#include "../parts/Clock.h"

#define DEV_SERIAL_POLOLU	"/dev/serial/by-id/usb-Pololu_Corporation_Pololu_Micro_Maestro_6-Servo_Controller_00097868-if00"

class R2D2 : public Maestro{

public:

	R2D2();
	void Shutdown();

private:

	Serial serial;

	void Init_serial();

	Autonomy	autonomy;
	Cameras		cameras;
	Clock		internal_clock;
	Logger		logger;
	Motors		motors;
	Remote_control	remote_control;
	Remote_monitor	remote_monitor;
	State_machine	state_machine;

};

#endif
