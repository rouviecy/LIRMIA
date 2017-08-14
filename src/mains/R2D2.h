/*
 * @(#)		R2D2.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * R2D2
 */

#ifndef R2D2_H
#define R2D2_H

#include "../core/Maestro.h"
#include "../interfaces/I2C.h"
#include "../interfaces/Serial.h"
#include "../parts/r2d2/Autonomy.h"
#include "../parts/r2d2/Cameras.h"
#include "../parts/r2d2/Logger.h"
#include "../parts/r2d2/Motors.h"
#include "../parts/r2d2/Remote_control.h"
#include "../parts/r2d2/Remote_monitor.h"
#include "../parts/r2d2/State.h"
#include "../parts/r2d2/Statetry.h"//-----------------------------------
#include "../parts/r2d2/State_machine.h"
#include "../parts/r2d2/Sonima.h"//-----------------------------------
#include "../parts/Clock.h"
#include "../parts/Compass_CMPS10.h"
#include "../parts/Depth_BMP085.h"
#include "../parts/Sonar.h"
#include "../parts/Gyro_ITG3200.h"

#define DEV_SERIAL_POLOLU	"/dev/serial/by-id/usb-Pololu_Corporation_Pololu_Micro_Maestro_6-Servo_Controller_00097868-if00"
#define DEV_I2C			"/dev/i2c-1"
#define DEV_SERIAL_RS232_SONAR	"/dev/serial/by-id/usb-Prolific_Technology_Inc._USB-Serial_Controller-if00-port0"
//#define DEV_SERIAL_RS232_SONAR	"usb-Prolific_Technology_Inc._USB_2.0_To_COM_Device-if00-port0"
class R2D2 : public Maestro{

public:

	R2D2();
	void Shutdown();

private:

	I2C i2c;
	Serial serial, serial_rs232_sonar;

	void Init_serial_and_i2c();

	Autonomy	autonomy;
	Cameras		cameras;
	Compass_CMPS10	compass;
	Sonima		sonima;//
	Clock		internal_clock;
	Depth_BMP085	depth;
	Gyro_ITG3200	gyro;
	Sonar		sonar;
	Logger		logger;
	Motors		motors;
	Remote_control	remote_control;
	Remote_monitor	remote_monitor;
	State		state;
	//Statetry	statetry;//----------------------------------------------
	State_machine	state_machine;

};

#endif
