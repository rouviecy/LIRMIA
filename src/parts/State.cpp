#include "State.h"

using namespace std;

State::State() : ComThread(){
	last_t = -1.;
	x = 0.; vx = 0.; thx = 0.; vthx = 0.; last_imu_thx = 0.;
	y = 0.; vy = 0.; thy = 0.; vthy = 0.; last_imu_thy = 0.;
	z = 0.; vz = 0.; thz = 0.; vthz = 0.; last_imu_thz = 0.;
}

State::~State(){}

void State::On_start(){}

void State::IO(){
	Link_input("t", &t);
	Link_input("imu_thx", &imu_thx);   Link_input("imu_thy", &imu_thy);   Link_input("imu_thz", &imu_thz);

	Link_output("x", &x);       Link_output("y", &y);       Link_output("z", &z);
	Link_output("vx", &vx);     Link_output("vy", &vy);     Link_output("vz", &vz);
	Link_output("thx", &thx);   Link_output("thy", &thy);   Link_output("thz", &thz);
	Link_output("vthx", &vthx); Link_output("vthy", &vthy); Link_output("vthz", &vthz);
}

void State::Job(){
	Critical_receive();
	thx = imu_thx;
	thy = imu_thy;
	thz = imu_thz;
	if(t < 0){
		last_t = t;
	}
	else if(t - last_t > 0.01){
		vthx = (imu_thx - last_imu_thx) / (t - last_t);
		vthy = (imu_thy - last_imu_thy) / (t - last_t);
		vthz = (imu_thz - last_imu_thz) / (t - last_t);
		last_t = t;
		last_imu_thx = imu_thx;
		last_imu_thy = imu_thy;
		last_imu_thz = imu_thz;
	}
	Critical_send();
}
