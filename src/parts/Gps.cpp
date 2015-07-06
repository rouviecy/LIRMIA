#include "Gps.h"

using namespace std;

Gps::Gps() : ComThread(){
	gps_xy[0] = 0.;
	gps_xy[1] = 0.;
	first = true;
}

Gps::~Gps(){}

void Gps::On_start(){}

void Gps::IO(){
	Link_output("gps_xy",	COMFLOAT, 2, gps_xy);
}

void Gps::Job(){} // Do nothing : this object should only be called by subscriber

void Gps::Process_serial_data(void* object, char* input_msg){
	Gps* self = (Gps*) object;
	string msg = string(input_msg);
	if(msg.size() < 37){return;}
	size_t next;
	vector <string> tokens;
	for(size_t current = 0; tokens.size() < 6; current = next + 1){
		next = msg.find_first_of(",", current);
		tokens.push_back(msg.substr(current, next - current));
	}
	if(	tokens[1].size() < 6 or
		tokens[2].size() < 7 or
		tokens[3].size() < 1 or
		tokens[4].size() < 7 or
		tokens[5].size() < 1){
			cout << "GPS ERROR" << endl;
			return;
	}
	try{
		int hh		= stoi(tokens[1].substr(0, 2));
		int mm		= stoi(tokens[1].substr(2, 2));
		int ss		= stoi(tokens[1].substr(4, 2));
		int lat_dd	= stoi(tokens[2].substr(0, 2));
		float lat_mm	= stof(tokens[2].substr(2, 7));
		int lon_dd	= stoi(tokens[4].substr(0, 3));
		float lon_mm	= stof(tokens[4].substr(3, 7));
		bool is_N	= (tokens[3].compare("N") == 0);
		bool is_W	= (tokens[5].compare("W") == 0);
		float lat_float = ((float) lat_dd) * M_PI / 180. + lat_mm * M_PI / 10800.;
		float lon_float = ((float) lon_dd) * M_PI / 180. + lon_mm * M_PI / 10800.;
		float x = (is_N ? +1. : -1.) * R_EARTH * cos(lat_float) * cos(lon_float);
		float y = (is_W ? -1. : +1.) * R_EARTH * cos(lat_float) * sin(lon_float);
		if(self->first){
			self->offset_x = x;
			self->offset_y = y;
			self->first = false;
		}
		self->gps_xy[0] = x - self->offset_x;
		self->gps_xy[1] = y - self->offset_y;
		cout << self->gps_xy[0] << "\t" << self->gps_xy[1] << endl;
		self->Critical_send();
	}
	catch(std::exception const & e){}
}

void Gps::Subscribe(Subscriber* subscriber){
	#if defined(ENABLE_GPS) and not defined(ENABLE_SERIAL_ARDUINO)
		cout << "[Warning] You are trying to use GPS without serial enabled : GPS will be disabled" << endl;
		return;
	#endif
	map <int, char> keys;
	keys[0] = '$';
	keys[1] = 'G';
	keys[2] = 'P';
	keys[3] = 'G';
	keys[4] = 'G';
	keys[5] = 'A';
	subscriber->Subscribe(keys, &(Gps::Process_serial_data), (void*) this);
}
