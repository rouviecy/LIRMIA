#include "Imu.h"

using namespace std;

Imu::Imu() : ComThread(){
	imu_thx = 0.;  imu_thy = 0.;  imu_thz = 0.;
	imu_vthx = 0.; imu_vthy = 0.; imu_vthz = 0.;
}

Imu::~Imu(){}

void Imu::On_start(){}

void Imu::IO(){
	Link_output("imu_thx", &imu_thx);   Link_output("imu_thy", &imu_thy);   Link_output("imu_thz", &imu_thz);
	Link_output("imu_vthx", &imu_vthx); Link_output("imu_vthy", &imu_vthy); Link_output("imu_vthz", &imu_vthz);
}

void Imu::Job(){
	Critical_send();
}
