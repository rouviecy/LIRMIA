#include "Lirmia.h"

using namespace std;

Lirmia::Lirmia() : Maestro(){

	// Warning : pass-by-reference to avoid slicing !
	Add_thread(&sensor,		"My sensor",			10000);		// 10 ms
	Add_thread(&state,		"My state manager",		100000);	// 100 ms
	Add_thread(&state_machine,	"My finite state machine",	1000000);	// 1 s

	Link_all();
	Draw("coms");
	Launch_all();

}

void Lirmia::Shutdown(){
	Join_all();
}

int main(){
	Lirmia robot;
	usleep(5000000);
	robot.Shutdown();
	return 0;
}
