#include "Acoustic_modem.h"

using namespace std;

Acoustic_modem::Acoustic_modem() : ComThread(){
	sub_is_underwater = false;
}

Acoustic_modem::~Acoustic_modem(){}

void Acoustic_modem::On_start(){
	#ifdef ENABLE_SERIAL_RS232_MODEM
		receive_go_on = true;
		buffer_pos = 0;
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
		while(input_flow.size() > 0){
			mu.lock();
			input_flow.pop();
			mu.unlock();
		}
	#endif
	Critical_send();
}

void Acoustic_modem::Get_acoustic_msg_loop(Acoustic_modem* self){
	while(self->receive_go_on){
		self->serial->Lock();
		for(char* msg = self->serial->Serial_read(); *msg != 0; msg += sizeof(char)){
			self->buffer[self->buffer_pos++] = *msg;
			if(self->buffer_pos == 4){
				self->buffer_pos = 0;
				MSG_MODEM msg_parse;
				msg_parse.addressee	= self->buffer[0] >> 5;
				msg_parse.header	= (self->buffer[0] << 4) >> 1;
				msg_parse.checksum	= self->buffer[0] % 2;
				msg_parse.data[0]	= self->buffer[1];
				msg_parse.data[1]	= self->buffer[2];
				msg_parse.data[2]	= self->buffer[3];
				self->mu.lock();
				self->input_flow.push(msg_parse);
				self->mu.unlock();
			}
		}
		self->serial->Unlock();
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
