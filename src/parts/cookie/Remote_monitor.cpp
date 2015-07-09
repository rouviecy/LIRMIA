#include "Remote_monitor.h"

using namespace std;

Remote_monitor::Remote_monitor() : ComThread(){
	#ifdef ENABLE_TCP
		if(!tcp_server.Configure(7241)){return;}
		cout << "Waiting client for remote monitor on port 7241 ..." << endl;
		while(tcp_server.Get_nb_clients() == 0){
			usleep(1000000);
		}
		cout << "Client connected" << endl;
	#endif
}

Remote_monitor::~Remote_monitor(){}

void Remote_monitor::On_start(){}

void Remote_monitor::IO(){
	Link_input("t",			COMFLOAT,	1, &t);
	Link_input("xyz",		COMFLOAT, 	3, xyz);
	Link_input("thxyz",		COMFLOAT, 	3, thxyz);
	Link_input("fsm_state",		COMINT,		1, &fsm_state);
	Link_input("fsm_unlocked",	COMBOOL,	1, &fsm_unlocked);
	Link_input("motor",		COMFLOAT,	4, motor);
}

void Remote_monitor::Job(){
	Critical_receive();
	string msg_monitor =
		to_string(t) + "|" +
		to_string(fsm_state) + "|" +
		to_string(fsm_unlocked) + "|" +
		to_string((int) (xyz[0] * 1000)) + "|" +
		to_string((int) (xyz[1] * 1000)) + "|" +
		to_string((int) (xyz[2] * 1000)) + "|" +
		to_string(thxyz[2]) + "|" +
		to_string(motor[0] * 100) + "|" +
		to_string(motor[1] * 100) + "|" +
		to_string(motor[2] * 100) + "|" +
		to_string(motor[3] * 100) + "|";
	tcp_server.Send(msg_monitor);
}
