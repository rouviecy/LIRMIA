#include "Lirmia.h"

using namespace std;

Lirmia::Lirmia() : Maestro(){

	// Warning : pass-by-reference to avoid slicing !
	Add_thread(&acoustic_modem,	"Acoustic modem",		1000000);	// 1 s
	Add_thread(&autonomy,		"Autonomy",			50000);		// 50 ms
	Add_thread(&cameras,		"Cameras",			100000);	// 100 ms
	Add_thread(&internal_clock,	"Clock",			1000);		// 1 ms
	Add_thread(&depth,		"Depth",			10000);		// 10 s
	Add_thread(&echosonder,		"Echo sonder",			100000);	// 100 ms
	Add_thread(&imu,		"Inertial Measurement Unit",	10000);		// 10 ms
	Add_thread(&logger,		"Logger",			1000000);	// 1 s
	Add_thread(&mapping,		"Mapping",			1000000);	// 1 s
	Add_thread(&motors,		"Motors",			100000);	// 100 ms
	Add_thread(&remote,		"Remote control",		100000);	// 100 ms
	Add_thread(&state,		"State",			20000);		// 20 ms
	Add_thread(&state_machine,	"Finite state machine",		40000);		// 40 s

	#ifdef ENABLE_SERIAL
		Init_serial();
		depth.Calibrate();
	#endif
	Link_all();
	mapping.Link_img(cameras.Get_img1(), cameras.Get_img2());
	Draw("coms");
	Launch_all();

}

void Lirmia::Shutdown(){
	Join_all();
	#ifdef DEV_SERIAL
		serial.Serial_close();
	#endif
}

void Lirmia::Init_serial(){
	#ifdef DEV_SERIAL
		unsigned char init_I2C_and_serial[5];
		serial.Serial_init(DEV_SERIAL, B115200);
		init_I2C_and_serial[0] = 0x5A;	// initial command
		init_I2C_and_serial[1] = 0x02;	// ISS mode
		init_I2C_and_serial[2] = 0x61;	// I2C 100kHz and serial
		init_I2C_and_serial[3] = 0x00;	// nothing
		init_I2C_and_serial[4] = 0x19;	// baud
		serial.Serial_write(init_I2C_and_serial, 5);
		motors.Set_serial(&serial);
		imu.Set_serial(&serial);
	#endif
}

int main(){
	Lirmia robot;
	usleep(50000000);
	robot.Shutdown();
	return 0;
}
