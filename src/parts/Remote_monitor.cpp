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
	Link_input("t", &t);
	Link_input("thx", &thx);   Link_input("thy", &thy);   Link_input("thz", &thz);
	Link_input("motor1", &motor1);
	Link_input("motor2", &motor2);
	Link_input("motor3", &motor3);
	Link_input("motor4", &motor4);
	Link_input("fsm_state", &fsm_state);
}

void Remote_monitor::Job(){
	Critical_receive();
	string msg_monitor =
		to_string(t)+ "|" +
		to_string(fsm_state) + "|" +
		to_string(thx) + "|" +
		to_string(thy) + "|" +
		to_string(thz) + "|" +
		to_string(motor1 * 100) + "|" +
		to_string(motor2 * 100) + "|" +
		to_string(motor3 * 100) + "|" +
		to_string(motor4 * 100) + "|";
	tcp_server.Send(msg_monitor);
}
