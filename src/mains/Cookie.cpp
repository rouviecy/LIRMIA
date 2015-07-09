#include "Cookie.h"

using namespace std;

Cookie::Cookie() : Maestro(){

	// Warning : pass-by-reference to avoid slicing !
	Add_thread(&autonomy,		"Autonomy",			50000);		// 50 ms
	Add_thread(&cameras,		"Cameras",			100000);	// 100 ms
	Add_thread(&compass,		"Compass",			100000);	// 100 ms
	Add_thread(&internal_clock,	"Clock",			500000);	// 500 ms
	Add_thread(&depth,		"Depth",			500000);	// 500 ms
	Add_thread(&logger,		"Logger",			1000000);	// 1 s
	Add_thread(&motors,		"Motors",			10000);		// 10 ms
	Add_thread(&remote_control,	"Remote control",		-1);		// manual loop
	Add_thread(&remote_monitor,	"Remote monitor",		100000);	// 100 ms
	Add_thread(&state,		"State",			10000);		// 10 ms
	Add_thread(&state_machine,	"State machine",		10000);		// 10 ms

	Init_serial_and_i2c();
	Link_all();
	remote_control.Set_blobs_obj(cameras.Get_blobs_obj());
	Draw("coms_cookie");
	usleep(1000000);
	Launch_all();
	remote_control.Job_and_wait_quit();

}

void Cookie::Shutdown(){
	Join_all();
	#ifdef ENABLE_SERIAL
		serial.Serial_close();
	#endif
	#ifdef ENABLE_I2C
		i2c.I2C_close();
	#endif
}

void Cookie::Init_serial_and_i2c(){
	#ifdef ENABLE_SERIAL
		serial.Serial_init(DEV_SERIAL_POLOLU, B9600, true);
		motors.Set_serial(&serial);
	#endif
	#ifdef ENABLE_I2C
		i2c.I2C_init(DEV_I2C);
		compass.Set_i2c(&i2c);
		depth.Set_i2c(&i2c);
	#endif
}

int main(){
	Cookie robot;
//	usleep(50000000);
	robot.Shutdown();
	return 0;
}
