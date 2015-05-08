#include "Logger.h"

using namespace std;

Logger::Logger() : ComThread(){

}

Logger::~Logger(){}

void Logger::On_start(){}

void Logger::IO(){
	Link_input("fsm_state",	COMINT,		1, &fsm_state);
	Link_input("t",		COMFLOAT,	1, &t);
	Link_input("xyz",	COMFLOAT,	3, xyz);
	Link_input("vxyz",	COMFLOAT,	3, vxyz);
	Link_input("thxyz",	COMFLOAT,	3, thxyz);
	Link_input("vthxyz",	COMFLOAT,	3, vthxyz);
}

void Logger::Job(){
	Critical_receive();
}
