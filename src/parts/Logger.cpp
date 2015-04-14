#include "Logger.h"

using namespace std;

Logger::Logger() : ComThread(){

}

Logger::~Logger(){}

void Logger::On_start(){}

void Logger::IO(){
	Link_input("t", &t);
	Link_input("fsm_state", &fsm_state);
	Link_input("x", &x);        Link_input("y", &y);        Link_input("z", &z);
	Link_input("vx", &vx);      Link_input("vy", &vy);      Link_input("vz", &vz);
	Link_input("thx", &thx);    Link_input("thy", &thy);    Link_input("thz", &thz);
	Link_input("vthx", &vthx);  Link_input("vthy", &vthy);  Link_input("vthz", &vthz);
}

void Logger::Job(){
	Critical_receive();
}
