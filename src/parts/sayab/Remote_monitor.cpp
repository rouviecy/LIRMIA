#include "Remote_monitor.h"

using namespace std;

Remote_monitor::Remote_monitor() : ComThread(){
	#ifdef ENABLE_TCP
		if(!tcp_server.Configure(5241)){return;}
		cout << "Waiting client for remote monitor on port 5241 ..." << endl;
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
	Link_input("xy",		COMFLOAT,	2, xy);
	Link_input("lat",		COMFLOAT,	1, &lat);
	Link_input("lon",		COMFLOAT,	1, &lon);
	Link_input("thz",		COMFLOAT,	1, &thz);
	Link_input("vthz",		COMFLOAT,	1, &vthz);
	Link_input("yawref",		COMFLOAT,	1, &yawref);
	Link_input("uw",		COMFLOAT,	1, &uw);
	Link_input("motor",		COMFLOAT,	1, &motor);
	Link_input("rudder",		COMFLOAT,	1, &rudder);
	Link_input("bow_thruster",	COMFLOAT,	1, &bow_thruster);
	Link_input("fsm_state",		COMINT,		1, &fsm_state);
	Link_input("fsm_unlocked",	COMBOOL,	1, &fsm_unlocked);
//	Link_input("msg_parse",		COMFLOAT,	4, &msg_parse);
}

void Remote_monitor::Job(){
	Critical_receive();
	string msg_monitor =
		to_string(t) + "|" +
		to_string(ti) + "|" +
		to_string(fsm_state) + "|" +
		to_string(fsm_unlocked) + "|" +
		to_string((int) (xy[0] * 1000)) + "|" +
		to_string((int) (xy[1] * 1000)) + "|" +
		to_string(thz) + "|" +
		to_string(vthz) + "|" +
		to_string(yawref) + "|" +
		to_string(uw) + "|" +
		to_string(motor * 85) + "|" +
		to_string(rudder * 65) + "|" +
		to_string(bow_thruster * 65) + "|" +
		to_string(lat) + "|" +
             	to_string(lon) + "|";
//		to_string(msgmod[0]) + "|" +;
//		to_string(msgmod[1]) + "|" +;
//		to_string(msgmod[2]) + "|" +;
//		to_string(msgmod[3]) + "|" +;
	tcp_server.Send(msg_monitor);
}
