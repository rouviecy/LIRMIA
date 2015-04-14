#include "Depth.h"

using namespace std;

Depth::Depth() : ComThread(){
	depth = 42.;
}

Depth::~Depth(){}

void Depth::On_start(){}

void Depth::IO(){
	Link_output("depth", &depth);
}

void Depth::Job(){
	Critical_send();
}
