#include "Remote.h"

using namespace std;

Remote::Remote() : ComThread(){
	remote = 0.;
	alive = false;
	#ifdef ENABLE_TCP
		alive = true;
		if(!tcp_server.Configure(4242)){return;}
		while(tcp_server.Get_nb_clients() == 0){
			usleep(1000000);
		}
		cout << "Client connected" << endl;
	#endif
}

Remote::~Remote(){}

void Remote::On_start(){}

void Remote::IO(){
	Link_output("remote", &remote);
}

void Remote::Job(){
	#ifdef ENABLE_TCP
		while(alive){
			char* msg_in = tcp_server.Receive(0);
			cout << msg_in << endl;
			if(msg_in[0] == 'b' && msg_in[1] == 'y' && msg_in[2] == 'e'){
				alive = false;
				tcp_server.Close();
			}
		}
	#endif
}
