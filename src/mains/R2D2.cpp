#include "R2D2.h"

using namespace std;

R2D2::R2D2() : Maestro(){

	// Warning : pass-by-reference to avoid slicing !
	Add_thread(&autonomy,		"Autonomy",			50000);		// 50 ms
	Add_thread(&cameras,		"Cameras",			100000);	// 100 ms
	Add_thread(&internal_clock,	"Clock",			1000);		// 1 ms
	Add_thread(&logger,		"Logger",			1000000);	// 1 s
	Add_thread(&motors,		"Motors",			10000);		// 10 ms
	Add_thread(&remote_control,	"Remote control",		-1);		// manual loop
	Add_thread(&remote_monitor,	"Remote monitor",		100000);	// 100 ms
	Add_thread(&state_machine,	"State machine",		10000);		// 10ms

	Init_serial();
	Link_all();
	remote_control.Set_blobs_obj(cameras.Get_blobs_obj());
	Draw("coms_r2d2");
	usleep(1000000);
	Launch_all();
	remote_control.Job_and_wait_quit();

}

void R2D2::Shutdown(){
	Join_all();
	#ifdef ENABLE_SERIAL
		serial.Serial_close();
	#endif
}

void R2D2::Init_serial(){
	#ifdef ENABLE_SERIAL
		serial.Serial_init(DEV_SERIAL_POLOLU, B9600, true);
		motors.Set_serial(&serial);
	#endif
}

int main(){
	R2D2 robot;
//	usleep(50000000);
	robot.Shutdown();
	return 0;
}
