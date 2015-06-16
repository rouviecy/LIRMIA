#include "State_machine.h"

using namespace std;

State_machine::State_machine() : ComThread(){

	fsm.Add_state("stay",			STAY);
	fsm.Add_state("down",			DOWN);
	fsm.Add_state("explore",		EXPLORE);
	fsm.Add_state("follow_obj_cam",		FOLLOW_OBJ_CAM);
	fsm.Add_state("follow_wall",		FOLLOW_WALL);
	fsm.Add_state("up",			UP);
	fsm.Add_state("remote",			REMOTE);

	fsm.Add_event("go_to_autonomous");
	fsm.Add_event("go_down");
	fsm.Add_event("begin_explore");
	fsm.Add_event("found_something_cam");
	fsm.Add_event("wall_detected");
	fsm.Add_event("stop_follow");
	fsm.Add_event("go_up");
	fsm.Add_event("reach_surface");
	fsm.Add_event("go_to_remote");

	fsm.Add_action("update_fsm", &(State_machine::Update_fsm));

	fsm.Add_guard("fsm_unlocked", &fsm_unlocked);

				// state_from		// state_to		// trigger		// guard	// action	// object
	// Go to stabilization
	fsm.Add_transition(	"remote",		"stay",			"go_to_autonomous",	"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"down",			"stay",			"go_to_autonomous",	"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"up",			"stay",			"go_to_autonomous",	"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"explore",		"stay",			"go_to_autonomous",	"fsm_unlocked",	"update_fsm",	(void*) this);
	// End

	// Go up and down
	fsm.Add_transition(	"stay",			"down",			"go_down",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"stay",			"up",			"go_up",		"fsm_unlocked",	"update_fsm",	(void*) this);
	// End

	// Between explore and follow
	fsm.Add_transition(	"explore",		"follow_obj_cam",	"found_something_cam",	"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"explore",		"follow_wall",		"wall_detected",	"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_obj_cam",	"explore",		"stop_follow",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_wall",		"explore",		"stop_follow",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"stay",			"explore",		"begin_explore",	"fsm_unlocked",	"update_fsm",	(void*) this);
	// End

	// Go back to remote chain
	fsm.Add_transition(	"follow_obj_cam",	"explore",		"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_wall",		"explore",		"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"explore",		"stay",			"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"down",			"stay",			"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"up",			"stay",			"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"stay",			"remote",		"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	// End

	fsm_state = REMOTE;
	fsm.Launch("remote");
	drawer.Draw_FSM("FSM_r2d2", &fsm);

}

State_machine::~State_machine(){}

void State_machine::On_start(){}

void State_machine::IO(){
	Link_input("remote",		COMBOOL,	1, &remote);
	Link_input("fsm_unlocked",	COMBOOL,	1, &fsm_unlocked);
	Link_input("fsm_stabilize",	COMBOOL,	1, &fsm_stabilize);
	Link_input("fsm_down",		COMBOOL,	1, &fsm_down);
	Link_input("fsm_up",		COMBOOL,	1, &fsm_up);
	Link_input("fsm_explore",	COMBOOL,	1, &fsm_explore);
	Link_input("fsm_nofollow",	COMBOOL,	1, &fsm_nofollow);
	Link_input("cam_detect_obj",	COMBOOL,	1, &cam_detect_obj);
	Link_output("fsm_state",	COMINT,		1, &fsm_state);
}

void State_machine::Job(){
	Critical_receive();
	// TODO : appel de l'événement "wall_detected"
	if(fsm_state != REMOTE && remote){fsm.Call_event("go_to_remote");}
	if(fsm_state == REMOTE && !remote || fsm_stabilize){fsm.Call_event("go_to_autonomous");}
	if(fsm_down){fsm.Call_event("go_down");}
	if(fsm_up){fsm.Call_event("go_up");}
	if(fsm_explore){fsm.Call_event("begin_explore");}
	if(fsm_nofollow){fsm.Call_event("stop_follow");}
	if(fsm_state == EXPLORE){
		if(fsm_up){fsm.Call_event("go_up");}
		if(cam_detect_obj && !fsm_nofollow){fsm.Call_event("found_something_cam");}
	}
	if(fsm_state == FOLLOW_OBJ_CAM && !cam_detect_obj){fsm.Call_event("stop_follow");}
	Critical_send();
}

string State_machine::Decode_state_str(int int_state){
	switch(int_state){
		case STAY:		return "stabilize position    ";
		case DOWN:		return "going down            ";
		case EXPLORE:		return "explore               ";
		case FOLLOW_OBJ_CAM:	return "follow object camera  ";
		case FOLLOW_WALL:	return "following wall        ";
		case UP:		return "going up              ";
		case REMOTE:		return "remote control        ";
		default :		return "unknown ???           ";
	}
}

void State_machine::Update_fsm(void* obj){
	State_machine* state_machine = (State_machine*) obj;
	state_machine->fsm_state = state_machine->fsm.Get_current_states_id()[0];
}
