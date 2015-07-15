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
	#ifdef ENABLE_SPI
		spi->Lock();
		unsigned char request[] = {0x00, 0x57, 0x00, 0x00, 0x00, 0x00};
//		spi->SPI_write(request, 6);
//		unsigned char* answer = spi->SPI_read(6);
		spi->SPI_duplex(request, 6);
		for(int i = 0; i < 6; i++){
			cout << (int) request[i] << " | ";
		}
		cout << endl;
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
