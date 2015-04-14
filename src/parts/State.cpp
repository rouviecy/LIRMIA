#include "State.h"

using namespace std;

State::State() : ComThread(){}

State::~State(){}

void State::On_start(){}

void State::IO(){
	Link_input("t", &t);
	Link_input("depth", &depth);
	Link_input("imu_thx", &imu_thx);   Link_input("imu_thy", &imu_thy);   Link_input("imu_thz", &imu_thz);
	Link_input("imu_vthx", &imu_vthx); Link_input("imu_vthy", &imu_vthy); Link_input("imu_vthz", &imu_vthz);

	Link_output("x", &x);       Link_output("y", &y);       Link_output("z", &z);
	Link_output("vx", &vx);     Link_output("vy", &vy);     Link_output("vz", &vz);
	Link_output("thx", &thx);   Link_output("thy", &thy);   Link_output("thz", &thz);
	Link_output("vthx", &vthx); Link_output("vthy", &vthy); Link_output("vthz", &vthz);
}

void State::Job(){
	Critical_receive();
	Critical_send();
}
