#include "Remote_control.h"

using namespace std;

Remote_control::Remote_control() : ComThread(){
	remote = true; remote_forward = 0.; remote_turn = 0.; remote_lateral = 0.;
	enable_streaming = false;
	alive = false;
	#ifdef ENABLE_TCP
		if(!tcp_server.Configure(5242)){return;}
		cout << "Waiting client for remote control on port 5242 ..." << endl;
		while(tcp_server.Get_nb_clients() == 0){
			usleep(1000000);
		}
		cout << "Client connected" << endl;
		alive = true;
	#endif
}

Remote_control::~Remote_control(){}

void Remote_control::On_start(){}

void Remote_control::IO(){
	Link_output("remote",		COMBOOL,	1, &remote);
	Link_output("remote_forward",	COMFLOAT,	1, &remote_forward);
	Link_output("remote_turn",	COMFLOAT,	1, &remote_turn);
	Link_output("remote_lateral",	COMFLOAT,	1, &remote_lateral);
	Link_output("enable_streaming",	COMBOOL,	1, &enable_streaming);
}

void Remote_control::Job(){}

void Remote_control::Job_and_wait_quit(){
	#ifdef ENABLE_TCP
		while(alive){
			char* msg_in = tcp_server.Receive(0);
			if(msg_in[0] == 'b' && msg_in[1] == 'y' && msg_in[2] == 'e'){
				alive = false;
				tcp_server.Close();
			}
			if(msg_in[0] == 'r'){
				if(msg_in[1] == '0'){remote = false;}
				if(msg_in[1] == '1'){remote = true;}
				if(msg_in[1] == 'a'){remote_forward =	(msg_in[2] == '1' ? +1. : 0.);}
				if(msg_in[1] == 'b'){remote_forward =	(msg_in[2] == '1' ? -1. : 0.);}
				if(msg_in[1] == 'l'){remote_turn =	(msg_in[2] == '1' ? -1. : 0.);}
				if(msg_in[1] == 'r'){remote_turn =	(msg_in[2] == '1' ? +1. : 0.);}
				if(msg_in[1] == 'w'){remote_lateral =	(msg_in[2] == '1' ? -1. : 0.);}
				if(msg_in[1] == 'e'){remote_lateral =	(msg_in[2] == '1' ? +1. : 0.);}
				if(msg_in[1] == 's'){enable_streaming =	(msg_in[2] == '1');}
			}
			if(msg_in[0] == 'h' && msg_in[1] == 's' && msg_in[2] == 'v' && msg_in[3] == '_'){
				int param = stoi(string(msg_in + 7 * sizeof(char)));
				if(msg_in[4] == 'h' and msg_in[5] == '0'){hsv.H_min = param;}
				if(msg_in[4] == 'h' and msg_in[5] == '1'){hsv.H_max = param;}
				if(msg_in[4] == 's' and msg_in[5] == '0'){hsv.S_min = param;}
				if(msg_in[4] == 's' and msg_in[5] == '1'){hsv.S_max = param;}
				if(msg_in[4] == 'v' and msg_in[5] == '0'){hsv.V_min = param;}
				if(msg_in[4] == 'v' and msg_in[5] == '1'){hsv.V_max = param;}
				if(msg_in[4] == 'm' and msg_in[5] == '0'){hsv.nb_dilate = param;}
				if(msg_in[4] == 'm' and msg_in[5] == '1'){hsv.nb_erode = param;}
				if(msg_in[4] == 't' and msg_in[5] == '0'){hsv.seuil = param;}
				blobs->Definir_limites_separation(&hsv);
			}
			Critical_send();
		}
	#endif
}

void Remote_control::Set_blobs_obj(Blobs* blobs){this->blobs = blobs;}
