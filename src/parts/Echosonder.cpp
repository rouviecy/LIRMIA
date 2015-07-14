#include "Echosonder.h"

using namespace std;

Echosonder::Echosonder() : ComThread(){
	obst_xy[0] = 42.;
	obst_xy[1] = 42.;
	request = 'Z';
}

Echosonder::~Echosonder(){}

void Echosonder::On_start(){}

void Echosonder::IO(){
	Link_output("obst_xy",	COMFLOAT, 2, obst_xy);
}

void Echosonder::Job(){
	#ifdef ENABLE_SERIAL_RS232_ECHO_X
		obst_xy[0] = Measure_distance(serial_x);
	#endif
	#ifdef ENABLE_SERIAL_RS232_ECHO_Y
		obst_xy[1] = Measure_distance(serial_y);
	#endif
	Critical_send();
}

float Echosonder::Measure_distance(Serial* serial){
	float result = -1;
	serial->Lock();
	serial->Serial_write(&request, 1);
	char* answer = serial->Serial_read();
	answer[7] = 0;
	try{result = stof(string(answer));}
	catch(std::exception const & e){}
	serial->Unlock();
	return result;
}

void Echosonder::Set_serial_x(Serial* serial){this->serial_x = serial;}
void Echosonder::Set_serial_y(Serial* serial){this->serial_y = serial;}
