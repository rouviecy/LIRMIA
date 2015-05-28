#include "Gps.h"

using namespace std;

Gps::Gps() : ComThread(){
	gps_xy[0] = 0.;
	gps_xy[1] = 0.;
}

Gps::~Gps(){}

void Gps::On_start(){}

void Gps::IO(){
	Link_output("gps_xy",	COMFLOAT, 2, gps_xy);
}

void Gps::Job(){
	// TODO
	Critical_send();
}

void Gps::Set_serial(Serial* serial){this->serial = serial;}
