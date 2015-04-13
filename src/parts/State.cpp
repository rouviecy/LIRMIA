#include "State.h"

using namespace std;

State::State() : ComThread(){}

State::~State(){}

void State::On_start(){}

void State::IO(){
	Link_input("my_data", &data_in);
}

void State::Job(){
	Critical_receive();
	cout << "Received data : " << data_in << endl;
}
