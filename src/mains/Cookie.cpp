#include "Cookie.h"

using namespace std;

Cookie::Cookie() : Maestro(){

	// Warning : pass-by-reference to avoid slicing !
	Add_thread(&autonomy,		"Autonomy",			50000);		// 50 ms
	Add_thread(&cameras,		"Cameras",			100000);	// 100 ms
	Add_thread(&compass,		"Compass",			100000);	// 100 ms
	Add_thread(&internal_clock,	"Clock",			500000);	// 500 ms
//	Add_thread(&depth,		"Depth",			500000);	// 500 ms
	Add_thread(&imu,		"IMU",				1000000);	// 100 ms
	Add_thread(&logger,		"Logger",			1000000);	// 1 s
	Add_thread(&motors,		"Motors",			10000);		// 10 ms
	Add_thread(&remote_control,	"Remote control",		-1);		// manual loop
	Add_thread(&remote_monitor,	"Remote monitor",		100000);	// 100 ms
	Add_thread(&state,		"State",			10000);		// 10 ms
	Add_thread(&state_machine,	"State machine",		10000);		// 10 ms

	Init_hardware_com();
	Link_all();
	remote_control.Set_blobs_obj(cameras.Get_blobs_obj());
	Draw("coms_cookie");
	usleep(1000000);
	Launch_all();
	remote_control.Job_and_wait_quit();

}

void Cookie::Shutdown(){
	Join_all();
	#ifdef ENABLE_SERIAL_POLOLU
		serial_pololu.Serial_close();
	#endif
	#ifdef ENABLE_SERIAL_RASPI
		serial_raspi.Serial_close();
	#endif
	#ifdef ENABLE_I2C
		i2c.I2C_close();
	#endif
	#ifdef ENABLE_SPI
		spi.SPI_close();
	#endif
}

void Cookie::Init_hardware_com(){
	#ifdef ENABLE_SERIAL_POLOLU
		serial_pololu.Serial_init(DEV_SERIAL_POLOLU, B9600, true);
		motors.Set_serial(&serial_pololu);
	#endif
	#ifdef ENABLE_SERIAL_RASPI
		serial_raspi.Serial_init(DEV_SERIAL_RASPI, B9600, true);
		imu.Set_serial(&serial_raspi);
	#endif
	#ifdef ENABLE_I2C
		i2c.I2C_init(DEV_I2C);
		compass.Set_i2c(&i2c);
//		depth.Set_i2c(&i2c);
	#endif
	#ifdef ENABLE_SPI
		spi.SPI_init(DEV_SPI, 300000, 0, 1, 8);
		imu.Set_spi(&spi);
	#endif
}

int main(){
	Cookie robot;
	robot.Shutdown();
	return 0;
}
