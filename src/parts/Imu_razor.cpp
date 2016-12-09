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

void Imu_razor::Job(){} // Do nothing : this object should only be called by subscriber

void Imu_razor::Process_serial_data(void* object, char* input_msg){
        Imu_razor* self = (Imu_razor*) object;
cout << "Received razor msg : " << input_msg << endl;
        stringstream ss;
        ss << string(input_msg).substr(5) << ",";
        vector <string> tokens;
        for(string token; getline(ss, token, ','); tokens.push_back(token)){}
        if(tokens.size() < 3 || tokens[0].length() < 3 || tokens[1].length() < 3 || tokens[2].length() < 3){return;}
	try{
		self->imu_thxyz[0] = stof(tokens[0]);
		self->imu_thxyz[1] = stof(tokens[1]);
		self->imu_thxyz[2] = stof(tokens[2]);
		self->Critical_send();
	}
	catch(const std::invalid_argument &e){
		cout << "Wrong razor msg : " << input_msg << endl << "\t" << e.what() << endl;
	}
}

void Imu_razor::Subscribe(Subscriber* subscriber){
	//#if defined(ENABLE_IMU) and not defined(ENABLE_SERIAL_ARDUINO)
	#if defined(ENABLE_IMU) and not defined(ENABLE_SERIAL_RS232_IMU)
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
