#include "Depth_MS5837.h"

using namespace std;

Depth_MS5837::Depth_MS5837() : ComThread(){
	depth = 0.;
}

Depth_MS5837::~Depth_MS5837(){}

void Depth_MS5837::On_start(){}

void Depth_MS5837::IO(){
	Link_input("t",		COMFLOAT, 1, &t);
	Link_output("depth",	COMFLOAT, 1, &depth);
}

void Depth_MS5837::Job(){} // Do nothing : this object should only be called by subscriber

void Depth_MS5837::Process_serial_data(void* object, char* input_msg){
	Depth_MS5837* self = (Depth_MS5837*) object;
//cout << "Received depth msg : " << input_msg << endl;
	string msg_ypr = string(input_msg + 5 * sizeof(char));
	if(count(msg_ypr.begin(), msg_ypr.end(), '$') == 1){
		vector <string> tokens;
		size_t next = msg_ypr.find_first_of("$", 0);
		string msg_depth = msg_ypr.substr(0, next);
		try{
			self->depth = stof(msg_depth);
			self->Critical_send();
		}
		catch(std::exception const & e){}
	}
}

void Depth_MS5837::Subscribe(Subscriber* subscriber){
	#if defined(ENABLE_DEPTH) and not defined(ENABLE_SERIAL_ARDUINO)
		cout << "[Warning] You are trying to use depth sensor without serial enabled : depth sensor will be disabled" << endl;
		return;
	#endif
	map <int, char> keys;
	keys[0] = '%';
	keys[1] = 'D';
	keys[2] = 'E';
	keys[3] = 'P';
	keys[4] = '=';
	subscriber->Subscribe(keys, &(Depth_MS5837::Process_serial_data), (void*) this);
}
