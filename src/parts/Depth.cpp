#include "Depth.h"

using namespace std;

Depth::Depth() : ComThread(){
	depth = 0.;
}

Depth::~Depth(){}

void Depth::On_start(){}

void Depth::IO(){
	Link_input("t",		COMFLOAT, 1, &t);
	Link_output("depth",	COMFLOAT, 1, &depth);
}

void Depth::Job(){
	cout << "[Warning] Depth thread should not be launched : only Subscriber should call it" << endl;
}

void Depth::Process_serial_data(void* object, char* input_msg){
	Depth* self = (Depth*) object;
	string msg_ypr = string(input_msg + 5 * sizeof(char));
	if(count(msg_ypr.begin(), msg_ypr.end(), ',') == 1){
		vector <string> tokens;
		size_t next = msg_ypr.find_first_of(",", 0);
		string msg_depth = msg_ypr.substr(0, next);
		try{
			self->depth = stof(msg_depth);
			self->Critical_send();
		}
		catch(std::exception const & e){}
	}
}

void Depth::Subscribe(Subscriber* subscriber){
	#if defined(ENABLE_DEPTH) and not defined(ENABLE_SERIAL_ARDUINO)
		cout << "[Warning] You are trying to use depth sensor without serial enabled : depth sensor will be disabled" << endl;
		return;
	#endif
	map <int, char> keys;
	keys[0] = '#';
	keys[1] = 'D';
	keys[2] = 'E';
	keys[3] = 'P';
	keys[4] = '=';
	subscriber->Subscribe(keys, &(Depth::Process_serial_data), (void*) this);
}
