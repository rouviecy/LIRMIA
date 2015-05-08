#include "Acoustic_modem.h"

using namespace std;

Acoustic_modem::Acoustic_modem() : ComThread(){

}

Acoustic_modem::~Acoustic_modem(){}

void Acoustic_modem::On_start(){}

void Acoustic_modem::IO(){
	Link_input("xyz",	COMFLOAT, 3, xyz);
	Link_input("thxyz",	COMFLOAT, 3, thxyz);
}

void Acoustic_modem::Job(){
	Critical_receive();
}
