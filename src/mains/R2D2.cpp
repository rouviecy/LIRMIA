#include "R2D2.h"

using namespace std;

R2D2::R2D2() : Maestro(){

	// Warning : pass-by-reference to avoid slicing !
	Add_thread(&autonomy,		"Autonomy",			50000);		// 50 ms
	Add_thread(&cameras,		"Cameras",			100000);	// 100 ms
	Add_thread(&compass,		"Compass",			100000);	// 100 ms
	Add_thread(&internal_clock,	"Clock",			500000);	// 500 ms
	Add_thread(&depth,		"Depth",			500000);	// 500 ms
	Add_thread(&sonar,		"Sonar",			25000);		// 25 ms
	Add_thread(&sonima,		"Sonima",			50000);		// 25 ms
//	Add_thread(&gyro,		"Gyro",				100000);	// 100 ms
	Add_thread(&logger,		"Logger",			1000000);	// 1 s
	Add_thread(&motors,		"Motors",			10000);		// 10 ms
	Add_thread(&remote_control,	"Remote control",		-1);		// manual loop
	Add_thread(&remote_monitor,	"Remote monitor",		100000);	// 100 ms
	Add_thread(&state,		"State",			10000);		// 10 ms
//	Add_thread(&statetry,		"Statetry",			10000);		// 10 ms
	Add_thread(&state_machine,	"State machine",		10000);		// 10 ms

	Init_serial_and_i2c();
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
	#ifdef ENABLE_I2C
		i2c.I2C_close();
	#endif
	#ifdef ENABLE_SERIAL_RS232_SONAR
		serial_rs232_sonar.Serial_close();
	#endif
}

void R2D2::Init_serial_and_i2c(){
	#ifdef ENABLE_SERIAL
		serial.Serial_init(DEV_SERIAL_POLOLU, B9600, true);
		motors.Set_serial(&serial);
	#endif
	#ifdef ENABLE_I2C
		i2c.I2C_init(DEV_I2C);
		compass.Set_i2c(&i2c);
		depth.Set_i2c(&i2c);
//		gyro.Set_i2c(&i2c);
	#endif
	#ifdef ENABLE_SERIAL_RS232_SONAR
		serial_rs232_sonar.Serial_init(DEV_SERIAL_RS232_SONAR, B115200, true);
		sonar.Set_serial_s(&serial_rs232_sonar);
	#endif
}

int main(){
	R2D2 robot;
	robot.Shutdown();
	return 0;
}
