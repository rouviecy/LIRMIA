#include "Imu_UM6LT.h"

using namespace std;

Imu_UM6LT::Imu_UM6LT() : ComThread(){
	imu_vthz = 0.;
}

Imu_UM6LT::~Imu_UM6LT(){}

void Imu_UM6LT::On_start(){}

void Imu_UM6LT::IO(){
	Link_output("imu_vthz", COMFLOAT, 1, &imu_vthz);
}

void Imu_UM6LT::Job(){
	#ifdef ENABLE_SPI
		spi->Lock();
		unsigned char request[] = {0x00, 0x5D};
		unsigned char answer[4];
		spi->SPI_duplex(answer, request);
		imu_vthz = (float) (short) (answer[2]) << 8 | answer[3]);
		spi->Unlock();
		Critical_send();
	#endif
}

void Imu_UM6LT::Set_spi(SPI* spi){
	#ifndef ENABLE_SPI
		cout << "[Warning] Trying to use UM6-LT without I2C enabled ; IMU will be disabled" << endl;
		return;
	#endif
	this->spi = spi;
}
