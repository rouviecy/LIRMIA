/*
 * @(#)		Cookie.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Cookie robot
 */

#ifndef COOKIE_H
#define COOKIE_H

#include "../core/Maestro.h"
#include "../interfaces/I2C.h"
#include "../interfaces/Serial.h"
#include "../parts/cookie/Autonomy.h"
#include "../parts/cookie/Cameras.h"
#include "../parts/cookie/Logger.h"
#include "../parts/cookie/Motors.h"
#include "../parts/cookie/Remote_control.h"
#include "../parts/cookie/Remote_monitor.h"
#include "../parts/cookie/State.h"
#include "../parts/cookie/State_machine.h"
#include "../parts/Clock.h"
#include "../parts/Compass_CMPS10.h"
#include "../parts/Depth_BMP085.h"
#include "../parts/Imu_UM6LT.h"

// TODO : check device addresses
#define DEV_SERIAL_POLOLU	"/dev/serial/by-id/usb-Pololu_Corporation_Pololu_Micro_Maestro_6-Servo_Controller_00097868-if00"
#define DEV_SERIAL_POLOLU	"/dev/ttyACM0"
#define DEV_I2C			"/dev/i2c-1"

class Cookie : public Maestro{

public:

	Cookie();
	void Shutdown();

private:

	I2C i2c;
	Serial serial_pololu, serial_raspi;

	void Init_serial_and_i2c();

	Autonomy	autonomy;
	Cameras		cameras;
	Compass_CMPS10	compass;
	Clock		internal_clock;
	Depth_BMP085	depth;
	Imu_UM6LT	imu;
	Logger		logger;
	Motors		motors;
	Remote_control	remote_control;
	Remote_monitor	remote_monitor;
	State		state;
	State_machine	state_machine;

};

#endif
