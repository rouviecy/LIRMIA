#include "Imu.h"

using namespace std;

Imu::Imu() : ComThread(){
	imu_thx = 0.;  imu_thy = 0.;  imu_thz = 0.;
	#if defined(ENABLE_IMU) and not defined(ENABLE_SERIAL_ARDUINO)
		cout << "[Warning] You are trying to use IMU without serial enabled : IMU will be disabled" << endl;
	#endif
}

Imu::~Imu(){}

void Imu::On_start(){}

void Imu::IO(){
	Link_output("imu_thx", &imu_thx);
	Link_output("imu_thy", &imu_thy);
	Link_output("imu_thz", &imu_thz);
}

void Imu::Job(){
	#if defined(ENABLE_IMU) && defined(ENABLE_SERIAL_ARDUINO)
		serial->Lock();
		char* answer = serial->Serial_read();
		for(int i = 0; i < 128; i++){
			cout << (int) answer[i] << " | ";
		}
			cout << endl;
		serial->Unlock();
	#endif
}

void Imu::Set_serial(Serial* serial){this->serial = serial;}
