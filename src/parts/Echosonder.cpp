#include "Echosonder.h"

using namespace std;

Echosonder::Echosonder() : ComThread(){
	obst_x = 42.;
	obst_y = 42.;
	obst_vx = 0.;
	obst_vy = 0.;
}

Echosonder::~Echosonder(){}

void Echosonder::On_start(){}

void Echosonder::IO(){
	Link_output("obst_x", &obst_x);   Link_output("obst_y", &obst_y);
	Link_output("obst_vx", &obst_vx); Link_output("obst_vy", &obst_vy);
}

void Echosonder::Job(){
	Critical_send();
}
