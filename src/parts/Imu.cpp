#include "Imu.h"

using namespace std;

Imu::Imu() : ComThread(){
	imu_thx = 0.;  imu_thy = 0.;  imu_thz = 0.;
}

Imu::~Imu(){}

void Imu::On_start(){}

void Imu::IO(){
	Link_output("imu_thx", &imu_thx);
	Link_output("imu_thy", &imu_thy);
	Link_output("imu_thz", &imu_thz);
}

void Imu::Job(){
	#if defined(ENABLE_SERIAL) && defined(ENABLE_IMU)
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
