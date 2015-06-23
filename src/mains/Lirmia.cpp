#include "Lirmia.h"

using namespace std;

Lirmia::Lirmia() : Maestro(){

	// Warning : pass-by-reference to avoid slicing !
	Add_thread(&acoustic_modem,	"Acoustic modem",		1000000);	// 1 s
	Add_thread(&autonomy,		"Autonomy",			50000);		// 50 ms
	Add_thread(&cameras,		"Cameras",			100000);	// 100 ms
	Add_thread(&internal_clock,	"Clock",			1000);		// 1 ms
	Add_thread(&echosonder,		"Echo sonder",			100000);	// 100 ms
	Add_thread(&depth,		"Depth",			-1);		// subscriber callback
	Add_thread(&imu,		"IMU",				-1);		// subscriber callback
	Add_thread(&logger,		"Logger",			1000000);	// 1 s
	Add_thread(&mapping,		"Mapping",			1000000);	// 1 s
	Add_thread(&motors,		"Motors",			10000);		// 10 ms
	Add_thread(&remote_control,	"Remote control",		-1);		// manual loop
	Add_thread(&remote_monitor,	"Remote monitor",		100000);	// 100 ms
	Add_thread(&simulator,		"Simulator",			5000);		// 5 ms
	Add_thread(&state,		"State",			20000);		// 20 ms
	Add_thread(&state_machine,	"Finite state machine",		40000);		// 40 ms
	Add_thread(&subscriber,		"Subscriber",			100000);	// 100 ms

	Init_serial();
	Link_all();
	remote_control.Set_blobs_obj(cameras.Get_blobs_obj());
//	mapping.Link_img(cameras.Get_img1(), cameras.Get_img2());
	Draw("coms_lirmia");
	usleep(1000000);
	Launch_all();
	remote_control.Job_and_wait_quit();

}

void Lirmia::Shutdown(){
	acoustic_modem.Stop_receive();
	Join_all();
	#ifdef ENABLE_SERIAL_ARDUINO
		serial_arduino.Serial_close();
	#endif
	#ifdef ENABLE_SERIAL_ISS
		serial_iss.Serial_close();
	#endif
	#ifdef ENABLE_SERIAL_RS232
		serial_rs232.Serial_close();
	#endif
}

void Lirmia::Init_serial(){
	#ifdef ENABLE_SERIAL_ARDUINO
		serial_arduino.Serial_init(DEV_SERIAL_ARDUINO, B57600, true);
		subscriber.Set_serial(&serial_arduino);
		imu.Subscribe(&subscriber);
		depth.Subscribe(&subscriber);
	#endif
	#ifdef ENABLE_SERIAL_ISS
		serial_iss.Serial_init(DEV_SERIAL_ISS, B115200, false);
		unsigned char init_I2C_and_serial[5];
		init_I2C_and_serial[0] = 0x5A;	// initial command
		init_I2C_and_serial[1] = 0x02;	// change
		init_I2C_and_serial[2] = 0x61;	// serial and I2C 100 kHz
		init_I2C_and_serial[3] = 0x00;	// baudrate
		init_I2C_and_serial[4] = 0x19;	// baudrate 115200
		serial_iss.Serial_write(init_I2C_and_serial, 5);
		motors.Set_serial(&serial_iss);
	#endif
	#ifdef ENABLE_SERIAL_RS232
		serial_rs232.Serial_init(DEV_SERIAL_RS232, B9600, true);
		acoustic_modem.Set_serial(&serial_rs232);
	#endif
}

int main(){
	Lirmia robot;
//	usleep(50000000);
	robot.Shutdown();
	return 0;
}
