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
#include "../parts/Depth_MS5803.h"
//#include "../parts/Depth_MS5803_iss.h"
#include "../parts/Echosonder.h"
#include "../parts/Imu_razor.h"
#include "../parts/Subscriber.h"


//#define DEV_SERIAL_ARDUINO	"/dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_5533330393435131F151-if00"
//#define DEV_SERIAL_ARDUINO      "/dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_95333303031351D0B2B1-if00"
#define DEV_SERIAL_ARDUINO	"/dev/serial/by-id/usb-Arduino_Srl_Arduino_Uno_85439303333351019211-if00"
//#define DEV_SERIAL_ARDUINO      "/dev/serial/by-id/usb-Arduino_Srl_Arduino_Uno_75430333137351C012E0-if00"
#define DEV_SERIAL_ISS		"/dev/serial/by-id/usb-Devantech_Ltd._USB-ISS._00007350-if00"
#define DEV_SERIAL_ISS_DEPTH	"/dev/serial/by-id/usb-Devantech_Ltd._USB-ISS._00014561-if00"
#define DEV_SERIAL_RS232_MODEM	"/dev/serial/by-id/usb-Prolific_Technology_Inc._USB-Serial_Controller-if00-port0"
#define DEV_SERIAL_RS232_ECHO_X	"/dev/serial/by-id/usb-FTDI_US232R_FTE4451C-if00-port0" // TO CHANGE
#define DEV_SERIAL_RS232_ECHO_Y	"/dev/serial/by-id/usb-FTDI_US232R_FTE4451C-if00-port0" // TO CHANGE
#define DEV_SERIAL_RS232_IMU 	"/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AH03FIKV-if00-port0"

class Lirmia : public Maestro{

public:

	Lirmia();
	void Shutdown();

private:

	Serial serial_arduino, serial_iss, serial_iss_depth, serial_rs232_modem, serial_rs232_echo_x, serial_rs232_echo_y, serial_imu;

	void Init_serial();

	Acoustic_modem	acoustic_modem;
	Autonomy		autonomy;
	Cameras			cameras;
	Clock			internal_clock;
	Depth_MS5803		depth;
//	Depth_MS5803_iss	depth;
	Echosonder		echosonder;
	Imu_razor		imu;
	Logger			logger;
	Mapping			mapping;
	Motors			motors;
	Remote_control		remote_control;
	Remote_monitor		remote_monitor;
	Simulator		simulator;
	State			state;
	State_machine		state_machine;
	Subscriber		subscriber;
	Subscriber		subscriber2;

};

#endif
