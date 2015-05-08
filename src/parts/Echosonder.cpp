#include "Echosonder.h"

using namespace std;

Echosonder::Echosonder() : ComThread(){
	obst_xy[0] = 42.;
	obst_xy[1] = 42.;
	obst_vxy[0] = 0.;
	obst_vxy[1] = 0.;
}

Echosonder::~Echosonder(){}

void Echosonder::On_start(){}

void Echosonder::IO(){
	Link_output("obst_xy",	COMFLOAT, 2, obst_xy);
	Link_output("obst_vxy",	COMFLOAT, 2, obst_vxy);
}

void Echosonder::Job(){
	Critical_send();
}
