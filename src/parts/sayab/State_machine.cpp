#include "State_machine.h"

using namespace std;

State_machine::State_machine() : ComThread(){

	fsm.Add_state("stay",			STAY);
	fsm.Add_state("remote",			REMOTE);
	fsm.Add_state("explore",		EXPLORE);
	fsm.Add_state("follow_cam_sub",		FOLLOW_CAM_SUB);
	fsm.Add_state("follow_cam_surf",	FOLLOW_CAM_SURF);
	fsm.Add_state("law_control",		LAW_CONTROL);
//	fsm.Add_state("lost_cam_sub",		LOST_CAM_SUB);
//	fsm.Add_state("lost_cam_surf",		LOST_CAM_SURF);

	fsm.Add_event("go_to_autonomous");
	fsm.Add_event("go_to_remote");
	fsm.Add_event("begin_explore");
	fsm.Add_event("follow_sub");
	fsm.Add_event("follow_surf");
	fsm.Add_event("go_to_law_control");
	fsm.Add_event("stop_follow");
//	fsm.Add_event("detected_sub");
//	fsm.Add_event("detected_surf");
//	fsm.Add_event("lost_sub");
//	fsm.Add_event("lost_surf");

	fsm.Add_action("update_fsm", &(State_machine::Update_fsm));

	fsm.Add_guard("fsm_unlocked", &fsm_unlocked);

				// state_from		// state_to		// trigger		// guard	// action	// object
	//Go to stabilization
	fsm.Add_transition(	"remote",		"stay",			"go_to_autonomous",	"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"law_control",		"stay",			"go_to_autonomous",	"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"explore",		"stay",			"go_to_autonomous",	"fsm_unlocked", "update_fsm",	(void*) this);
	//end

	//Go to law control
	fsm.Add_transition(	"stay",			"law_control",		"go_to_law_control", 	"fsm_unlocked",	"update_fsm",	(void*) this);
	//end

	//Between explore and follow
	fsm.Add_transition(	"explore",		"follow_cam_surf",	"follow_surf",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"explore",		"follow_cam_sub",	"follow_sub",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_cam_surf",	"explore",		"stop_follow",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_cam_sub",	"explore",		"stop_follow",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"stay",			"explore",		"begin_explore",	"fsm_unlocked",	"update_fsm",	(void*) this);
	//end

	//Go back to remote chain
	fsm.Add_transition(	"follow_cam_surf",	"explore",		"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_cam_sub",	"explore",		"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"explore",		"stay",			"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"law_control",		"stay",			"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"stay",			"remote",		"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*)	this);
	//end


	fsm_state = REMOTE;
	fsm.Launch("remote");
	drawer.Draw_FSM("FSM_sayab", &fsm);

}

State_machine::~State_machine(){}

void State_machine::On_start(){}

void State_machine::IO(){
	Link_input("remote",		COMBOOL,	1, &remote);
	Link_input("fsm_unlocked",	COMBOOL,	1, &fsm_unlocked);
	Link_input("fsm_stabilize",	COMBOOL,	1, &fsm_stabilize);
	Link_input("cam_detect_obj",	COMBOOL,	2, cam_detect_obj);
//	Link_input("sub_is_underwater",	COMBOOL,	1, &sub_is_underwater);
	Link_input("fsm_law_control",	COMBOOL,	1, &fsm_law_control);
	Link_input("fsm_explore",	COMBOOL,	1, &fsm_explore);
	Link_input("fsm_nofollow",	COMBOOL,	1, &fsm_nofollow);

	Link_output("fsm_state",	COMINT,		1, &fsm_state);
}

void State_machine::Job(){
	Critical_receive();
//		if(cam_detect_obj[0] || cam_detect_obj[1]){
//		switch(fsm_state){
//			case EXPLORE:		fsm.Call_event("go_to_autonomous");	break;
//			case STAY:		fsm.Call_event("go_to_law_control");	break;
//			case FOLLOW_CAM_SUB:	fsm.Call_event("stop_follow");		break;
//			case FOLLOW_CAM_SURF:	fsm.Call_event("stop_follow");		break;
//			default:							break;
//			}
//		}
	if(fsm_state != REMOTE		&& remote)		{fsm.Call_event("go_to_remote");}
	if(fsm_state == REMOTE		&& !remote || fsm_stabilize){fsm.Call_event("go_to_autonomous");}
	if(fsm_law_control){fsm.Call_event("go_to_law_control");}
	if(fsm_explore){fsm.Call_event("begin_explore");}
	if(fsm_nofollow){fsm.Call_event("stop_follow");}
	if(fsm_state == EXPLORE){
		if(fsm_law_control){fsm.Call_event("go_to_law_control");}
		if(!fsm_nofollow && !cam_detect_obj[1]){fsm.Call_event("follow_sub");}
		if(!fsm_nofollow && !cam_detect_obj[0]){fsm.Call_event("follow_surf");}
	}
	if(fsm_state == FOLLOW_CAM_SUB){
		if(cam_detect_obj[1]){fsm.Call_event("follow_sub");}
		if(!cam_detect_obj[1]){fsm.Call_event("stop_follow");}
	}
	if(fsm_state == FOLLOW_CAM_SURF){
		if(cam_detect_obj[0]){fsm.Call_event("follow_surf");}
		if(!cam_detect_obj[0]){fsm.Call_event("stop_follow");}
	}
	Critical_send();
}

string State_machine::Decode_state_str(int int_state){
	switch(int_state){
		case STAY:		return "stabilize position  ";
		case EXPLORE:		return "explore		    ";
		case REMOTE:		return "remote control      ";
		case FOLLOW_CAM_SUB:	return "follow cam submarine";
		case FOLLOW_CAM_SURF:	return "follow cam surface  ";
		case LAW_CONTROL:	return "law control	    ";
		default :		return "unknown ???         ";
	}
}

void State_machine::Update_fsm(void* obj){
	State_machine* state_machine = (State_machine*) obj;
	state_machine->fsm_state = state_machine->fsm.Get_current_states_id()[0];
}
