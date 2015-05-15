#include "Acoustic_modem.h"

using namespace std;

Acoustic_modem::Acoustic_modem() : ComThread(){

// TODO : ENABLE_SERIAL_RS232 ET ENABLE_MODEM


	// Common state
	fsm.Add_state("idle",			IDLE);

	// Master states
	fsm.Add_state("master_bad_com",		MASTER_BAD_COM);
	fsm.Add_state("master_good_com",	MASTER_GOOD_COM);
	fsm.Add_state("master_want_x",		MASTER_WANT_X);
	fsm.Add_state("master_want_y",		MASTER_WANT_Y);
	fsm.Add_state("master_want_z",		MASTER_WANT_Z);
	fsm.Add_state("master_want_ypr",	MASTER_WANT_YPR);
	fsm.Add_state("master_want_command",	MASTER_WANT_COMMAND);
	fsm.Add_state("master_confirm_x",	MASTER_CONFIRM_X);
	fsm.Add_state("master_confirm_y",	MASTER_CONFIRM_Y);
	fsm.Add_state("master_confirm_z",	MASTER_CONFIRM_Z);
	fsm.Add_state("master_confirm_ypr",	MASTER_CONFIRM_YPR);
	fsm.Add_state("master_confirm_command",	MASTER_CONFIRM_COMMAND);

	// Slave states
	fsm.Add_state("slave_good_com",		SLAVE_GOOD_COM);
	fsm.Add_state("slave_improve_angle",	SLAVE_IMPROVE_ANGLE);
	fsm.Add_state("slave_improve_distance",	SLAVE_IMPROVE_DISTANCE);
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
	fsm.Add_event("receive_data");
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
	fsm.Add_action("send_request_x",			&(Acoustic_modem::Send_request_x));
	fsm.Add_action("send_request_y",			&(Acoustic_modem::Send_request_y));
	fsm.Add_action("send_request_z",			&(Acoustic_modem::Send_request_z));
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
	fsm.Add_transition(	"master_good_com",		"master_want_x",		"want_xyz",			"",		"send_request_x",			(void*) this);
	fsm.Add_transition(	"master_want_x",		"master_confirm_x",		"receive_data",			"",		"send_same_msg",			(void*) this);
	fsm.Add_transition(	"master_confirm_x",		"master_bad_com",		"receive_wrong_msg",		"",		"send_key_and_stabilize",		(void*) this);
	fsm.Add_transition(	"master_confirm_x",		"master_want_y",		"receive_confirm_msg",		"",		"send_request_y",			(void*) this);
	fsm.Add_transition(	"master_want_y",		"master_confirm_y",		"receive_data",			"",		"send_same_msg",			(void*) this);
	fsm.Add_transition(	"master_confirm_y",		"master_bad_com",		"receive_wrong_msg",		"",		"send_key_and_stabilize",		(void*) this);
	fsm.Add_transition(	"master_confirm_y",		"master_want_z",		"receive_confirm_msg",		"",		"send_request_z",			(void*) this);
	fsm.Add_transition(	"master_want_z",		"master_confirm_z",		"receive_data",			"",		"send_same_msg",			(void*) this);
	fsm.Add_transition(	"master_confirm_z",		"master_bad_com",		"receive_wrong_msg",		"",		"send_key_and_stabilize",		(void*) this);
	fsm.Add_transition(	"master_confirm_z",		"idle",				"receive_confirm_msg",		"",		"back_to_idle",				(void*) this);

	// Master wants YPR
	fsm.Add_transition(	"master_good_com",		"master_want_ypr",		"want_ypr",			"",		"send_request_ypr",			(void*) this);
	fsm.Add_transition(	"master_want_ypr",		"master_confirm_ypr",		"receive_data",			"",		"send_same_msg",			(void*) this);
	fsm.Add_transition(	"master_confirm_ypr",		"master_bad_com",		"receive_wrong_msg",		"",		"send_key_and_stabilize",		(void*) this);
	fsm.Add_transition(	"master_confirm_ypr",		"idle",				"receive_confirm_msg",		"",		"back_to_idle",				(void*) this);

	// Master wants to command
	fsm.Add_transition(	"master_good_com",		"master_want_command",		"want_command",			"",		"send_request_command",			(void*) this);
	fsm.Add_transition(	"master_want_command",		"master_confirm_command",	"receive_data",			"",		"send_same_msg",			(void*) this);
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

	fsm_state = IDLE;
	fsm.Launch("idle");
	drawer.Draw_FSM("FSM_com", &fsm);

}

Acoustic_modem::~Acoustic_modem(){}

void Acoustic_modem::On_start(){}

void Acoustic_modem::IO(){
	Link_input("xyz",	COMFLOAT, 3, xyz);
	Link_input("thxyz",	COMFLOAT, 3, thxyz);
}

void Acoustic_modem::Job(){
	Critical_receive();
}

void Acoustic_modem::Back_to_idle(void* obj){}

void Acoustic_modem::Send_key_and_stabilize(void* obj){}
void Acoustic_modem::Send_request_x(void* obj){}
void Acoustic_modem::Send_request_y(void* obj){}
void Acoustic_modem::Send_request_z(void* obj){}
void Acoustic_modem::Send_request_ypr(void* obj){}
void Acoustic_modem::Send_request_command(void* obj){}
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
