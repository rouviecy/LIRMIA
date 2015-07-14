#include "Acoustic_modem.h"

using namespace std;

Acoustic_modem::Acoustic_modem() : ComThread(){
	sub_is_underwater = false;
}

Acoustic_modem::~Acoustic_modem(){}

void Acoustic_modem::On_start(){
	#ifdef ENABLE_SERIAL_RS232_MODEM
		receive_go_on = true;
		thr_reception = thread(&Acoustic_modem::Get_acoustic_msg_loop, this);
	#endif
}

void Acoustic_modem::IO(){
	Link_input("fsm_state",			COMINT,		1, &fsm_state);

	Link_output("sub_is_underwater",	COMBOOL,	1, &sub_is_underwater);
}

void Acoustic_modem::Job(){
	Critical_receive();
	#ifdef ENABLE_SERIAL_RS232_MODEM
		bool valid_key = false;
		while(input_flow.size() > 0){
			if(!valid_key){
				valid_key = (input_flow.front() == '#');
				input_flow.pop();
				continue;
			}
			else{
				switch(input_flow.front()){
					case 'u':
						sub_is_underwater = false;
						break;
					case 'd':
						sub_is_underwater = true;
						break;
					default:
						break;
				}
				input_flow.pop();
			}
		}
	#endif
	Critical_send();
}

void Acoustic_modem::Get_acoustic_msg_loop(){
	while(receive_go_on){
		serial->Lock();
		for(char* msg = serial->Serial_read(); *msg != '\0'; msg += sizeof(char)){
			input_flow.push(*msg);
		}
		serial->Unlock();
		usleep(10000);
	}
}

void Acoustic_modem::Send_acoustic_msg(std::string msg){
	serial->Lock();
	serial->Serial_write((unsigned char*) msg.c_str(), 4);
	serial->Unlock();
}

void Acoustic_modem::Stop_receive(){receive_go_on = false; thr_reception.join();}

void Acoustic_modem::Set_serial(Serial* serial){this->serial = serial;}
