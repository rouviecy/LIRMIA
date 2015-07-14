#include "Imu_UM6LT.h"

using namespace std;

Imu_UM6LT::Imu_UM6LT() : ComThread(){
	imu_thxyz[0] = 0.;
	imu_thxyz[1] = 0.;
	imu_thxyz[2] = 0.;
}

Imu_UM6LT::~Imu_UM6LT(){}

void Imu_UM6LT::On_start(){}

void Imu_UM6LT::IO(){
	Link_output("imu_thxyz", COMFLOAT, 3, imu_thxyz);
}

void Imu_UM6LT::Job(){
	cout << serial->Serial_read() << endl;
}

void Imu_UM6LT::Set_serial(Serial* serial){this->serial = serial;}
