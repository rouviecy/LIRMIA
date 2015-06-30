#include "State.h"

using namespace std;

State::State() : ComThread(){
	thxyz[0] = 0.;
	thxyz[1] = 0.;
	thxyz[2] = 0.;
}

State::~State(){}

void State::On_start(){}

void State::IO(){
	Link_input("compass_thxyz",	COMFLOAT, 3, compass_thxyz);

	Link_output("thxyz",		COMFLOAT, 3, thxyz);
}

void State::Job(){
	Critical_receive();
	thxyz[0] = compass_thxyz[0];
	thxyz[1] = compass_thxyz[1];
	thxyz[2] = compass_thxyz[2];
	Critical_send();
}
