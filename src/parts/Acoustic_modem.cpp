#include "Acoustic_modem.h"

using namespace std;

Acoustic_modem::Acoustic_modem() : ComThread(){

}

Acoustic_modem::~Acoustic_modem(){}

void Acoustic_modem::On_start(){}

void Acoustic_modem::IO(){
	Link_input("x", &x);     Link_input("y", &y);     Link_input("z", &z);
	Link_input("thx", &thx); Link_input("thy", &thy); Link_input("thz", &thz);
}

void Acoustic_modem::Job(){
	Critical_receive();
}
