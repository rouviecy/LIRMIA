#include "Clock.h"

using namespace std;

Clock::Clock() : ComThread(){
	start = chrono::high_resolution_clock::now();
}

Clock::~Clock(){}

void Clock::On_start(){}

void Clock::IO(){
	Link_output("t", &t);
}

void Clock::Job(){
	Critical_receive();
	now = chrono::high_resolution_clock::now();
	chrono::duration<float> delta = now - start;
	chrono::duration_cast<chrono::seconds>(delta);
	t = delta.count();
	Critical_send();
}
