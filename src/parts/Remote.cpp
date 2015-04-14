#include "Remote.h"

using namespace std;

Remote::Remote() : ComThread(){
	remote = 0.;
}

Remote::~Remote(){}

void Remote::On_start(){}

void Remote::IO(){
	Link_output("remote", &remote);
}

void Remote::Job(){
	Critical_send();
}
