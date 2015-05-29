#include "State_machine.h"

using namespace std;

State_machine::State_machine() : ComThread(){

	fsm.Add_state("remote",			REMOTE);
	fsm.Add_state("follow_cam_sub",		FOLLOW_CAM_SUB);
	fsm.Add_state("follow_cam_surf",	FOLLOW_CAM_SURF);
	fsm.Add_state("lost_cam_sub",		LOST_CAM_SUB);
	fsm.Add_state("lost_cam_surf",		LOST_CAM_SURF);

	fsm.Add_event("go_to_autonomous");
	fsm.Add_event("go_to_remote");
	fsm.Add_event("detected_sub");
	fsm.Add_event("detected_surf");
	fsm.Add_event("lost_sub");
	fsm.Add_event("lost_surf");
	fsm.Add_event("begin_follow_sub");
	fsm.Add_event("begin_follow_surf");

	fsm.Add_action("update_fsm", &(State_machine::Update_fsm));

				// state_from		// state_to		// trigger		// guard	// action	// object
	fsm.Add_transition(	"remote",		"follow_cam_surf",	"go_to_autonomous",	"",		"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_cam_surf",	"follow_cam_sub",	"begin_follow_sub",	"",		"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_cam_surf",	"lost_cam_surf",	"lost_surf",		"",		"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_cam_sub",	"follow_cam_surf",	"begin_follow_surf",	"",		"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_cam_sub",	"lost_cam_sub",		"lost_sub",		"",		"update_fsm",	(void*) this);
	fsm.Add_transition(	"lost_cam_surf",	"follow_cam_surf",	"detected_surf",	"",		"update_fsm",	(void*) this);
	fsm.Add_transition(	"lost_cam_sub",		"follow_cam_sub",	"detected_sub",		"",		"update_fsm",	(void*) this);

	fsm.Add_transition(	"follow_cam_surf",	"remote",		"go_to_remote",		"",		"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_cam_sub",	"remote",		"go_to_remote",		"",		"update_fsm",	(void*) this);
	fsm.Add_transition(	"lost_cam_surf",	"remote",		"go_to_remote",		"",		"update_fsm",	(void*) this);
	fsm.Add_transition(	"lost_cam_sub",		"remote",		"go_to_remote",		"",		"update_fsm",	(void*) this);


	fsm_state = REMOTE;
	fsm.Launch("remote");
	drawer.Draw_FSM("FSM_unknown", &fsm);

}

State_machine::~State_machine(){}

void State_machine::On_start(){}

void State_machine::IO(){
	Link_input("remote",		COMBOOL,	1, &remote);
	Link_input("cam_detect_obj",	COMBOOL,	2, cam_detect_obj);
	Link_input("sub_is_underwater",	COMBOOL,	1, &sub_is_underwater);

	Link_output("fsm_state",	COMINT,		1, &fsm_state);
}

void State_machine::Job(){
	Critical_receive();
	if(fsm_state == REMOTE		&& !remote)		{fsm.Call_event("go_to_autonomous");}
	if(fsm_state != REMOTE		&& remote)		{fsm.Call_event("go_to_remote");}
	if(fsm_state == FOLLOW_CAM_SUB	&& !cam_detect_obj[1])	{fsm.Call_event("lost_sub");}
	if(fsm_state == FOLLOW_CAM_SURF	&& !cam_detect_obj[0])	{fsm.Call_event("lost_surf");}
	if(fsm_state == LOST_CAM_SUB	&& cam_detect_obj[1])	{fsm.Call_event("detected_sub");}
	if(fsm_state == LOST_CAM_SURF	&& cam_detect_obj[0])	{fsm.Call_event("detected_surf");}
	if(fsm_state == FOLLOW_CAM_SUB	&& !sub_is_underwater)	{fsm.Call_event("begin_follow_surf");}
	if(fsm_state == FOLLOW_CAM_SURF	&& sub_is_underwater)	{fsm.Call_event("begin_follow_sub");}
	Critical_send();
}

string State_machine::Decode_state_str(int int_state){
	switch(int_state){
		case REMOTE:		return "remote control      ";
		case FOLLOW_CAM_SUB:	return "follow cam submarine";
		case FOLLOW_CAM_SURF:	return "follow cam surface  ";
		case LOST_CAM_SUB:	return "lost cam submarine  ";
		case LOST_CAM_SURF:	return "lost cam surface    ";
		default :		return "unknown ???         ";
	}
}

void State_machine::Update_fsm(void* obj){
	State_machine* state_machine = (State_machine*) obj;
	state_machine->fsm_state = state_machine->fsm.Get_current_states_id()[0];
}
