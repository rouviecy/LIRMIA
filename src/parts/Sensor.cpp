#include "Sensor.h"

using namespace std;

Sensor::Sensor() : ComThread(){
	data_out = 0.;
}

Sensor::~Sensor(){}

void Sensor::On_start(){}

void Sensor::IO(){
	Link_output("my_data", &data_out);
}

void Sensor::Job(){
	data_out += 1.;
	Critical_send();
}
