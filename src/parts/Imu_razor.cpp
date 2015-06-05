#include "Imu_razor.h"

using namespace std;

Imu_razor::Imu_razor() : ComThread(){
	imu_thxyz[0] = 0.;
	imu_thxyz[1] = 0.;
	imu_thxyz[2] = 0.;
}

Imu_razor::~Imu_razor(){}

void Imu_razor::On_start(){}

void Imu_razor::IO(){
	Link_output("imu_thxyz", COMFLOAT, 3, imu_thxyz);
}

void Imu_razor::Job(){
	cout << "[Warning] IMU thread should not be launched : only Subscriber should call it" << endl;
}

void Imu_razor::Process_serial_data(void* object, char* input_msg){
	Imu_razor* self = (Imu_razor*) object;
	string msg_ypr = string(input_msg + 5 * sizeof(char));
	if(count(msg_ypr.begin(), msg_ypr.end(), ',') == 3){
		vector <string> tokens;
		size_t next;
		for(size_t current = 0; tokens.size() < 3; current = next + 1){
			next = msg_ypr.find_first_of(",", current);
			tokens.push_back(msg_ypr.substr(current, next - current));
		}
		if(tokens[0].length() > 2 && tokens[1].length() > 2 && tokens[2].length() > 2){
			for(int i = 0; i < 3; i++){(self->imu_thxyz)[i] = stof(tokens[i]);}
			self->Critical_send();
		}
	}
}

void Imu_razor::Subscribe(Subscriber* subscriber){
	#if defined(ENABLE_IMU) and not defined(ENABLE_SERIAL_ARDUINO)
		cout << "[Warning] You are trying to use IMU without serial enabled : IMU will be disabled" << endl;
		return;
	#endif
	map <int, char> keys;
	keys[0] = '#';
	keys[1] = 'Y';
	keys[2] = 'P';
	keys[3] = 'R';
	keys[4] = '=';
	subscriber->Subscribe(keys, &(Imu_razor::Process_serial_data), (void*) this);
}
