#include "Remote.h"

using namespace std;

Remote::Remote() : ComThread(){
	remote = 0.;
	alive = false;
	#ifdef ENABLE_TCP
		if(!tcp_server.Configure(4242)){return;}
		cout << "Waiting client ..." << endl;
		while(tcp_server.Get_nb_clients() == 0){
			usleep(1000000);
		}
		cout << "Client connected" << endl;
		alive = true;
	#endif
}

Remote::~Remote(){}

void Remote::On_start(){}

void Remote::IO(){
	Link_output("remote", &remote);
	Link_output("remote_forward", &remote_forward);
	Link_output("remote_turn", &remote_turn);
	Link_output("remote_deeper", &remote_deeper);
}

void Remote::Job(){}

void Remote::Job_and_wait_quit(){
	#ifdef ENABLE_TCP
		while(alive){
			char* msg_in = tcp_server.Receive(0);
			cout << msg_in << endl;
			if(msg_in[0] == 'b' && msg_in[1] == 'y' && msg_in[2] == 'e'){
				alive = false;
				tcp_server.Close();
			}
			if(msg_in[0] = 'r'){
				if(msg_in[1] == '0'){remote = -1.;}
				if(msg_in[1] == '1'){remote = +1.;}
				if(msg_in[1] == 'a'){remote_forward =	(msg_in[2] == '1' ? +1. : 0.);}
				if(msg_in[1] == 'b'){remote_forward =	(msg_in[2] == '1' ? -1. : 0.);}
				if(msg_in[1] == 'l'){remote_turn =	(msg_in[2] == '1' ? -1. : 0.);}
				if(msg_in[1] == 'r'){remote_turn =	(msg_in[2] == '1' ? +1. : 0.);}
				if(msg_in[1] == 'u'){remote_deeper =	(msg_in[2] == '1' ? -1. : 0.);}
				if(msg_in[1] == 'd'){remote_deeper =	(msg_in[2] == '1' ? +1. : 0.);}	
			}
			Critical_send();
		}
	#endif	
}
