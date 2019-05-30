#include "Remote_monitor.h"

using namespace std;

Remote_monitor::Remote_monitor() : ComThread(){
	#ifdef ENABLE_TCP
		if(!tcp_server.Configure(4241)){return;}
		cout << "Waiting client for remote monitor on port 4241 ..." << endl;
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
	Link_input("ti",		COMFLOAT,	1, &ti);
	Link_input("xyz",		COMFLOAT,	3, xyz);
//	Link_input("vxyz",		COMFLOAT,	3, vxyz);
	Link_input("vz",		COMFLOAT,	1, &vz);
	Link_input("thxyz",		COMFLOAT,	3, thxyz);
	Link_input("vthz",		COMFLOAT,	1, &vthz);
//	Link_input("vthxyz",		COMFLOAT,	3, vthxyz);
	Link_input("yawref",		COMFLOAT,	1, &yawref);
	Link_input("mydwxy",		COMFLOAT, 	4, mydwxy);
	Link_input("zref",		COMFLOAT,	1, &zref);
	Link_input("uw",		COMFLOAT,	1, &uw);
	Link_input("uz",		COMFLOAT,	1, &uz);
	Link_input("motor",		COMFLOAT,	4, motor);
	Link_input("fsm_state",		COMINT,		1, &fsm_state);
	Link_input("fsm_unlocked",	COMBOOL,	1, &fsm_unlocked);
}

void Remote_monitor::Job(){
	Critical_receive();
	string msg_monitor =
		to_string(t) + "|" +
		to_string(ti) + "|" +
		to_string(fsm_state) + "|" +
		to_string(fsm_unlocked) + "|" +
		to_string((int) (xyz[0] * 1000)) + "|" +
		to_string((int) (xyz[1] * 1000)) + "|" +
		to_string(xyz[2]) + "|" +
//		to_string(vxyz[2]) + "|" +
		to_string(vz) + "|" +
		to_string(thxyz[2]) + "|" +
		to_string(vthz) + "|" +
//		to_string(vthxyz[2]) + "|" +
		to_string(yawref) + "|" +
		to_string(mydwxy[0]) + "|" +
		to_string(mydwxy[1]) + "|" +
		to_string(mydwxy[2]) + "|" +
		to_string(mydwxy[3]) + "|" +
		to_string(zref) + "|" +
		to_string(uw) + "|" +
		to_string(uz) + "|" +
		to_string(motor[0] * 100)+  + "|" +
		to_string(motor[1] * 100) + "|" +
		to_string(motor[2] * 100) + "|" +
		to_string(motor[3] * 100) + "|" ;

	tcp_server.Send(msg_monitor);
}
