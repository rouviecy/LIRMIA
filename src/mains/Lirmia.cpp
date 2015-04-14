#include "Lirmia.h"

using namespace std;

Lirmia::Lirmia() : Maestro(){

	// Warning : pass-by-reference to avoid slicing !
	Add_thread(&acoustic_modem,	"Acoustic modem",	1000000);	// 1 s
	Add_thread(&autonomy,		"Autonomy",		50000);		// 50 ms
	Add_thread(&cameras,		"Cameras",		1000000);	// 1 s
	Add_thread(&internal_clock,	"Clock",		1000);		// 1 ms
	Add_thread(&depth,		"Depth",		10000);		// 10 s
	Add_thread(&echosonder,		"Echo sonder",		100000);	// 100 ms
	Add_thread(&imu,		"Acoustic modem",	10000);		// 10 ms
	Add_thread(&logger,		"Logger",		1000000);	// 1 s
	Add_thread(&mapping,		"Mapping",		1000000);	// 1 s
	Add_thread(&motors,		"Motors",		100000);	// 100 ms
	Add_thread(&remote,		"Remote control",	100000);	// 100 ms
	Add_thread(&state,		"State",		20000);		// 20 ms
	Add_thread(&state_machine,	"Finite state machine",	40000);		// 40 s

	Link_all();
//	mapping.Link_img(cameras.Get_img1(), cameras.Get_img2());
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
