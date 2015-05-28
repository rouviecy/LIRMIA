#include "Acoustic_modem.h"

using namespace std;

Acoustic_modem::Acoustic_modem() : ComThread(){

	fsm_state = IDLE;

	last_t_msg = -1.;
	need_xyz = false;
	need_ypr = false;
	receive_go_on = false;

	modem_txyz[0] = -1.; modem_txyz[1] = 0.; modem_txyz[2] = 0.; modem_txyz[3] = 0.;
	modem_typr[0] = -1.; modem_typr[1] = 0.; modem_typr[2] = 0.; modem_typr[3] = 0.;

	modem_stab = false;
	for(int i = 0; i < 3; i++){
		order_xyz_com[i] = 0.;
		order_thxyz_com[i] = 0.;
	}

	#if defined(ENABLE_MODEM) and not defined(ENABLE_SERIAL_RS232)
		cout << "[Warning] You are trying to use acoustic modem without serial : modem will be disabled" << endl;
	#else

		// Common state
		fsm.Add_state("idle",			IDLE);

		// Master states
		fsm.Add_state("master_bad_com",		MASTER_BAD_COM);
		fsm.Add_state("master_good_com",	MASTER_GOOD_COM);
		fsm.Add_state("master_want_x",		MASTER_WANT_X);
		fsm.Add_state("master_want_y",		MASTER_WANT_Y);
		fsm.Add_state("master_want_z",		MASTER_WANT_Z);
		fsm.Add_state("master_want_ypr",	MASTER_WANT_YPR);
		fsm.Add_state("master_confirm_xyz",	MASTER_CONFIRM_XYZ);
		fsm.Add_state("master_confirm_ypr",	MASTER_CONFIRM_YPR);
		fsm.Add_state("master_confirm_command",	MASTER_CONFIRM_COMMAND);

		// Slave states
		fsm.Add_state("slave_improve_angle",	SLAVE_IMPROVE_ANGLE);
		fsm.Add_state("slave_improve_distance",	SLAVE_IMPROVE_DISTANCE);
		fsm.Add_state("slave_good_com",		SLAVE_GOOD_COM);
		fsm.Add_state("slave_send_x",		SLAVE_SEND_X);
		fsm.Add_state("slave_send_y",		SLAVE_SEND_Y);
		fsm.Add_state("slave_send_z",		SLAVE_SEND_Z);
		fsm.Add_state("slave_send_ypr",		SLAVE_SEND_YPR);
		fsm.Add_state("slave_exec_command",	SLAVE_EXEC_COMMAND);

		// Master events
		fsm.Add_event("want_something");
		fsm.Add_event("receive_good_key_confirm");
		fsm.Add_event("want_xyz");
		fsm.Add_event("want_ypr");
		fsm.Add_event("want_command");
		fsm.Add_event("receive_good_data");
		fsm.Add_event("receive_confirm_msg");
		fsm.Add_event("receive_wrong_msg");

		// Slave events
		fsm.Add_event("receive_bad_key");
		fsm.Add_event("receive_good_key");
		fsm.Add_event("best_orientation");
		fsm.Add_event("receive_request_xyz");
		fsm.Add_event("receive_request_ypr");
		fsm.Add_event("receive_request_command");
		fsm.Add_event("receive_good_confirm");
		fsm.Add_event("receive_wrong_confirm");

		// Common actions
		fsm.Add_action("back_to_idle",				&(Acoustic_modem::Back_to_idle));

		// Master actions
		fsm.Add_action("send_key_and_stabilize",		&(Acoustic_modem::Send_key_and_stabilize));
		fsm.Add_action("send_request_xyz",			&(Acoustic_modem::Send_request_xyz));
		fsm.Add_action("send_request_ypr",			&(Acoustic_modem::Send_request_ypr));
		fsm.Add_action("send_request_command",			&(Acoustic_modem::Send_request_command));
		fsm.Add_action("send_same_msg",				&(Acoustic_modem::Send_same_msg));

		// Slave actions
		fsm.Add_action("stabilize_without_obliquity",		&(Acoustic_modem::Stabilize_without_obliquity));
		fsm.Add_action("stabilize_with_little_obliquity",	&(Acoustic_modem::Stabilize_with_little_obliquity));
		fsm.Add_action("change_precession",			&(Acoustic_modem::Change_precession));
		fsm.Add_action("decrease_distance",			&(Acoustic_modem::Decrease_distance));
		fsm.Add_action("send_x",				&(Acoustic_modem::Send_x));
		fsm.Add_action("send_y",				&(Acoustic_modem::Send_y));
		fsm.Add_action("send_z",				&(Acoustic_modem::Send_z));
		fsm.Add_action("send_ypr",				&(Acoustic_modem::Send_ypr));
		fsm.Add_action("exec_command",				&(Acoustic_modem::Exec_command));
		fsm.Add_action("send_confirm_msg",			&(Acoustic_modem::Send_confirm_msg));
		fsm.Add_action("send_wrong_msg",			&(Acoustic_modem::Send_wrong_msg));

					// state_from			// state_to			// trigger			// guard	// action				// object
		// Master init
		fsm.Add_transition(	"idle",				"master_bad_com",		"want_something",		"",		"send_key_and_stabilize",		(void*) this);
		fsm.Add_transition(	"master_bad_com",		"master_bad_com",		"want_something",		"",		"send_key_and_stabilize",		(void*) this);
		fsm.Add_transition(	"master_bad_com",		"master_good_com",		"receive_good_key_confirm",	"",		"",					(void*) this);

		// Master wants XYZ
		fsm.Add_transition(	"master_good_com",		"master_want_x",		"want_xyz",			"",		"send_request_xyz",			(void*) this);
		fsm.Add_transition(	"master_want_x",		"master_want_y",		"receive_good_data",		"",		"send_same_msg",			(void*) this);
		fsm.Add_transition(	"master_want_y",		"master_want_z",		"receive_good_data",		"",		"send_same_msg",			(void*) this);
		fsm.Add_transition(	"master_want_z",		"master_confirm_xyz",		"receive_good_data",		"",		"send_same_msg",			(void*) this);
		fsm.Add_transition(	"master_want_x",		"master_bad_com",		"receive_wrong_msg",		"",		"send_key_and_stabilize",		(void*) this);
		fsm.Add_transition(	"master_want_y",		"master_bad_com",		"receive_wrong_msg",		"",		"send_key_and_stabilize",		(void*) this);
		fsm.Add_transition(	"master_want_z",		"master_bad_com",		"receive_wrong_msg",		"",		"send_key_and_stabilize",		(void*) this);
		fsm.Add_transition(	"master_confirm_xyz",		"master_bad_com",		"receive_wrong_msg",		"",		"send_key_and_stabilize",		(void*) this);
		fsm.Add_transition(	"master_confirm_xyz",		"idle",				"receive_confirm_msg",		"",		"back_to_idle",				(void*) this);

		// Master wants YPR
		fsm.Add_transition(	"master_good_com",		"master_want_ypr",		"want_ypr",			"",		"send_request_ypr",			(void*) this);
		fsm.Add_transition(	"master_want_ypr",		"master_confirm_ypr",		"receive_good_data",		"",		"send_same_msg",			(void*) this);
		fsm.Add_transition(	"master_want_ypr",		"master_bad_com",		"receive_wrong_msg",		"",		"send_key_and_stabilize",		(void*) this);
		fsm.Add_transition(	"master_confirm_ypr",		"master_bad_com",		"receive_wrong_msg",		"",		"send_key_and_stabilize",		(void*) this);
		fsm.Add_transition(	"master_confirm_ypr",		"idle",				"receive_confirm_msg",		"",		"back_to_idle",				(void*) this);

		// Master wants to command
		fsm.Add_transition(	"master_good_com",		"master_confirm_command",	"want_command",			"",		"send_request_command",			(void*) this);
		fsm.Add_transition(	"master_confirm_command",	"master_bad_com",		"receive_wrong_msg",		"",		"send_key_and_stabilize",		(void*) this);
		fsm.Add_transition(	"master_confirm_command",	"idle",				"receive_confirm_msg",		"",		"back_to_idle",				(void*) this);

		// Slave init
		fsm.Add_transition(	"idle",				"slave_good_com",		"receive_good_key",		"",		"stabilize_without_obliquity",		(void*) this);
		fsm.Add_transition(	"idle",				"slave_improve_angle",		"receive_bad_key",		"",		"stabilize_with_little_obliquity",	(void*) this);

		// Slave improve communication
		fsm.Add_transition(	"slave_improve_angle",		"slave_good_com",		"receive_good_key",		"",		"",					(void*) this);
		fsm.Add_transition(	"slave_improve_distance",	"slave_good_com",		"receive_good_key",		"",		"",					(void*) this);
		fsm.Add_transition(	"slave_improve_angle",		"slave_improve_angle",		"receive_bad_key",		"",		"change_precession",			(void*) this);
		fsm.Add_transition(	"slave_improve_angle",		"slave_improve_distance",	"best_orientation",		"",		"decrease_distance",		(void*) this);
		fsm.Add_transition(	"slave_improve_distance",	"slave_improve_distance",	"receive_bad_key",		"",		"decrease_distance",			(void*) this);

		// Slave sends XYZ
		fsm.Add_transition(	"slave_good_com",		"slave_send_x",			"receive_request_xyz",		"",		"send_x",				(void*) this);
		fsm.Add_transition(	"slave_send_x",			"slave_send_y",			"receive_good_confirm",		"",		"send_y",				(void*) this);
		fsm.Add_transition(	"slave_send_y",			"slave_send_z",			"receive_good_confirm",		"",		"send_z",				(void*) this);
		fsm.Add_transition(	"slave_send_z",			"idle",				"receive_good_confirm",		"",		"back_to_idle",			(void*) this);
		fsm.Add_transition(	"slave_send_x",			"slave_improve_angle",		"receive_wrong_confirm",	"",		"stabilize_with_little_obliquity",	(void*) this);
		fsm.Add_transition(	"slave_send_y",			"slave_improve_angle",		"receive_wrong_confirm",	"",		"stabilize_with_little_obliquity",	(void*) this);
		fsm.Add_transition(	"slave_send_z",			"slave_improve_angle",		"receive_wrong_confirm",	"",		"stabilize_with_little_obliquity",	(void*) this);

		// Slave sends YPR
		fsm.Add_transition(	"slave_good_com",		"slave_send_ypr",		"receive_request_ypr",		"",		"send_ypr",				(void*) this);
		fsm.Add_transition(	"slave_send_ypr",		"idle",				"receive_good_confirm",		"",		"back_to_idle",			(void*) this);
		fsm.Add_transition(	"slave_send_ypr",		"slave_improve_angle",		"receive_wrong_confirm",	"",		"stabilize_with_little_obliquity",	(void*) this);

		// Slave exec command
		fsm.Add_transition(	"slave_good_com",		"slave_exec_command",		"receive_request_command",	"",		"exec_command",				(void*) this);
		fsm.Add_transition(	"slave_exec_command",		"idle",				"receive_good_confirm",		"",		"back_to_idle",			(void*) this);
		fsm.Add_transition(	"slave_exec_command",		"slave_improve_angle",		"receive_wrong_confirm",	"",		"stabilize_with_little_obliquity",	(void*) this);

		fsm.Launch("idle");
		drawer.Draw_FSM("FSM_com", &fsm);

	#endif

}

Acoustic_modem::~Acoustic_modem(){}

void Acoustic_modem::On_start(){
	receive_go_on = true;
	thr_reception = thread(&Acoustic_modem::Get_acoustic_msg_loop, this);
}

void Acoustic_modem::IO(){
	Link_input("t",			COMFLOAT,	1, &t);
	Link_input("xyz",		COMFLOAT,	3, xyz);
	Link_input("thxyz",		COMFLOAT,	3, thxyz);

	Link_output("fsm_modem",	COMINT,		1, &fsm_state);
	Link_output("modem_txyz",	COMFLOAT,	4, modem_txyz);
	Link_output("modem_typr",	COMFLOAT,	4, modem_txyz);
	Link_output("modem_stab",	COMBOOL,	1, &modem_stab);
	Link_output("order_xyz_com",	COMFLOAT,	3, order_xyz_com);
	Link_output("order_thxyz_com",	COMFLOAT,	3, order_thxyz_com);
}

void Acoustic_modem::Job(){
	Critical_receive();
	#if defined(ENABLE_MODEM) and defined(ENABLE_SERIAL_RS232)
		switch(fsm_state){
			case IDLE:
				if(command_buffer.size() > 0 || need_xyz || need_ypr){
					for(int i = 0; i < 3; i++){
						order_xyz_com[i] = xyz[i];
						order_thxyz_com[i] = thxyz[i];
					}
					fsm.Call_event("want_something");
				}
				break;
			case MASTER_BAD_COM:
				while(in_msg_queue.size() > 0){
					if(Compare_msg(in_msg_queue.front(), ACOUSTIC_MSG_KEY)){
						fsm.Call_event("receive_good_key_confirm");
						break;
					}
					in_msg_queue.pop();
				}
				if(t - last_t_msg < ACOUSTIC_DT_REPEAT_KEY){
					last_t_msg = t;
					fsm.Call_event("want_something");
				}
				break;
			case MASTER_GOOD_COM:
				if	(command_buffer.size() > 0)	{fsm.Call_event("want_command");								break;}
				if	(need_xyz)			{fsm.Call_event("want_xyz");		mu.lock(); need_xyz = false;		mu.unlock();	break;}
				if	(need_ypr)			{fsm.Call_event("want_ypr");		mu.lock(); need_ypr = false;		mu.unlock();	break;}
				break;
			case MASTER_WANT_X:	Process_data(ACOUSTIC_HEADER_X, &(tmp_xyz[1]));	break;
			case MASTER_WANT_Y:	Process_data(ACOUSTIC_HEADER_Y, &(tmp_xyz[2]));	break;
			case MASTER_WANT_Z:	Process_data(ACOUSTIC_HEADER_Z, &(tmp_xyz[3]));	break;
			case MASTER_CONFIRM_XYZ:
				if(in_msg_queue.size() > 0){
					last_msg = in_msg_queue.front();
					in_msg_queue.pop();
					if(Compare_msg(last_msg, ACOUSTIC_CONFIRM_XYZ)){
						modem_txyz[0] = t;
						modem_txyz[1] = tmp_xyz[0];
						modem_txyz[2] = tmp_xyz[1];
						modem_txyz[3] = tmp_xyz[2];
						Critical_send();
						fsm.Call_event("receive_confirm_msg");
					}
					else{
						fsm.Call_event("receive_wrong_confirm");
					}
				}
				break;
			case MASTER_WANT_YPR:
				if(in_msg_queue.size() > 0){
					last_msg = in_msg_queue.front();
					in_msg_queue.pop();
					if(last_msg[0] == ACOUSTIC_HEADER_YPR){
						tmp_ypr[0] = Decode_acoustic_msg(in_msg_queue.front(), 1, 1) * 0.706 - 90.;
						tmp_ypr[1] = Decode_acoustic_msg(in_msg_queue.front(), 2, 1) * 1.412 - 180.;
						tmp_ypr[2] = Decode_acoustic_msg(in_msg_queue.front(), 3, 1) * 1.412 - 180.;
						fsm.Call_event("receive_good_data");
					}
					else{
						fsm.Call_event("receive_wrong_confirm");
					}
				}
				break;
			case MASTER_CONFIRM_YPR:
				if(in_msg_queue.size() > 0){
					last_msg = in_msg_queue.front();
					in_msg_queue.pop();
					if(Compare_msg(last_msg, ACOUSTIC_CONFIRM_YPR)){
						modem_typr[0] = t;
						modem_typr[1] = tmp_ypr[0];
						modem_typr[2] = tmp_ypr[1];
						modem_typr[3] = tmp_ypr[2];
						Critical_send();
						fsm.Call_event("receive_confirm_msg");
					}
					else{
						fsm.Call_event("receive_wrong_confirm");
					}
				}
				break;
			case MASTER_CONFIRM_COMMAND:
				if(in_msg_queue.size() > 0){
					last_msg = in_msg_queue.front();
					in_msg_queue.pop();
					string expected_answer;
					switch(command_buffer.front()){
						case COMMAND_REMOTE_ONOFF:	expected_answer = ACOUSTIC_MSG_COMMAND_REMOTE_ONOFF;	break;
						case COMMAND_STAY:		expected_answer = ACOUSTIC_MSG_COMMAND_STAY;		break;
						case COMMAND_EXPLORE:		expected_answer = ACOUSTIC_MSG_COMMAND_EXPLORE;		break;
						case COMMAND_NOFOLLOW:		expected_answer = ACOUSTIC_MSG_COMMAND_NOFOLLOW;	break;
						case COMMAND_UP:		expected_answer = ACOUSTIC_MSG_COMMAND_UP;		break;
						case COMMAND_DOWN:		expected_answer = ACOUSTIC_MSG_COMMAND_DOWN;		break;
						default:			expected_answer = "";					break;
					}
					if(Compare_msg(last_msg, expected_answer)){
						modem_typr[0] = t;
						modem_typr[1] = tmp_ypr[0];
						modem_typr[2] = tmp_ypr[1];
						modem_typr[3] = tmp_ypr[2];
						Critical_send();
						command_buffer.pop();
						fsm.Call_event("receive_confirm_msg");
					}
					else{
						fsm.Call_event("receive_wrong_confirm");
					}
				}
				break;
			default:
				cout << "[Warning] Undefined state for acoustic communication FSM" << endl;
				break;
		}
	#endif
}

void Acoustic_modem::Get_xyz(){mu.lock(); need_xyz = true; mu.unlock();}
void Acoustic_modem::Get_ypr(){mu.lock(); need_ypr = true; mu.unlock();}
void Acoustic_modem::Send_command(command_t command){
	if(command_buffer.size() < MAX_COMMANDS_IN_BUFFER){
		mu.lock();
		command_buffer.push(command);
		mu.unlock();
	}
}

bool Acoustic_modem::Compare_msg(string msg1, string msg2){
	if(msg1.length() < 4 || msg2.length() < 4){return false;}
	return msg1.compare(msg2) == 0;
}

float Acoustic_modem::Decode_acoustic_msg(std::string msg, int begin, int end){
	float result = 0.;
	try{result = stof(msg.substr(begin, end));
	}catch(std::exception const & e){}
	return result;
}

void Acoustic_modem::Process_data(char wanted_header, float* destination){
	if(in_msg_queue.size() > 0){
		last_msg = in_msg_queue.front();
		in_msg_queue.pop();
		if(in_msg_queue.front()[0] == wanted_header){
			*destination = Decode_acoustic_msg(last_msg, 1, 3);
			fsm.Call_event("receive_good_data");
		}
		else{
			fsm.Call_event("receive_wrong_confirm");
		}
	}
}

void Acoustic_modem::Get_acoustic_msg_loop(){
	while(receive_go_on){
		serial->Lock();
		char* msg = serial->Serial_read();
//cout << msg << endl;
//		in_msg_queue.push(string(msg));
		serial->Unlock();
	}
}

void Acoustic_modem::Send_acoustic_msg(std::string msg){
	serial->Lock();
	serial->Serial_write((unsigned char*) msg.c_str(), 4);
	serial->Unlock();
}

// TODO ...

void Acoustic_modem::Back_to_idle(void* obj){
	Acoustic_modem* self = (Acoustic_modem*) obj;
	self->modem_stab = false;
}

void Acoustic_modem::Send_key_and_stabilize(void* obj){
	Acoustic_modem* self = (Acoustic_modem*) obj;
	self->modem_stab = true;
	self->Critical_send();
	self->Send_acoustic_msg(ACOUSTIC_MSG_KEY);
}

void Acoustic_modem::Send_request_xyz(void* obj){
	Acoustic_modem* self = (Acoustic_modem*) obj;
	self->Send_acoustic_msg(ACOUSTIC_MSG_REQUEST_XYZ);
}

void Acoustic_modem::Send_request_ypr(void* obj){
	Acoustic_modem* self = (Acoustic_modem*) obj;
	self->Send_acoustic_msg(ACOUSTIC_MSG_REQUEST_YPR);	
}

void Acoustic_modem::Send_request_command(void* obj){
	Acoustic_modem* self = (Acoustic_modem*) obj;
	switch(self->command_buffer.front()){
		case COMMAND_REMOTE_ONOFF:	self->Send_acoustic_msg(ACOUSTIC_MSG_COMMAND_REMOTE_ONOFF);	break;
		case COMMAND_STAY:		self->Send_acoustic_msg(ACOUSTIC_MSG_COMMAND_STAY);		break;
		case COMMAND_EXPLORE:		self->Send_acoustic_msg(ACOUSTIC_MSG_COMMAND_EXPLORE);		break;
		case COMMAND_NOFOLLOW:		self->Send_acoustic_msg(ACOUSTIC_MSG_COMMAND_NOFOLLOW);		break;
		case COMMAND_UP:		self->Send_acoustic_msg(ACOUSTIC_MSG_COMMAND_UP);		break;
		case COMMAND_DOWN:		self->Send_acoustic_msg(ACOUSTIC_MSG_COMMAND_DOWN);		break;
		default:											break;
	}
}

void Acoustic_modem::Send_same_msg(void* obj){}

void Acoustic_modem::Stabilize_without_obliquity(void* obj){}
void Acoustic_modem::Stabilize_with_little_obliquity(void* obj){}
void Acoustic_modem::Change_precession(void* obj){}
void Acoustic_modem::Decrease_distance(void* obj){}
void Acoustic_modem::Send_x(void* obj){}
void Acoustic_modem::Send_y(void* obj){}
void Acoustic_modem::Send_z(void* obj){}
void Acoustic_modem::Send_ypr(void* obj){}
void Acoustic_modem::Exec_command(void* obj){}
void Acoustic_modem::Send_confirm_msg(void* obj){}
void Acoustic_modem::Send_wrong_msg(void* obj){}

void Acoustic_modem::Stop_receive(){receive_go_on = false; thr_reception.join();}

void Acoustic_modem::Set_serial(Serial* serial){this->serial = serial;}
