#include "State_machine.h"

using namespace std;

State_machine::State_machine() : ComThread(){

	fsm.Add_state("stay",			STAY);
	fsm.Add_state("down",			DOWN);
	fsm.Add_state("explore",		EXPLORE);
	fsm.Add_state("follow_obj_cam1",	FOLLOW_OBJ_CAM1);
	fsm.Add_state("follow_obj_cam2",	FOLLOW_OBJ_CAM2);
	fsm.Add_state("follow_pipe_cam1",	FOLLOW_PIPE_CAM1);
	fsm.Add_state("follow_pipe_cam2",	FOLLOW_PIPE_CAM2);
	fsm.Add_state("follow_wall",		FOLLOW_WALL);
	fsm.Add_state("up",			UP);
	fsm.Add_state("remote",			REMOTE);

	fsm.Add_event("go_to_autonomous");
	fsm.Add_event("go_down");
	fsm.Add_event("begin_explore");
	fsm.Add_event("found_something_cam1");
	fsm.Add_event("found_something_cam2");
	fsm.Add_event("pipe_detected_cam1");
	fsm.Add_event("pipe_detected_cam2");
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
	fsm.Add_transition(	"explore",		"follow_obj_cam1",	"found_something_cam1",	"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"explore",		"follow_obj_cam2",	"found_something_cam2",	"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"explore",		"follow_pipe_cam1",	"pipe_detected_cam1",	"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"explore",		"follow_pipe_cam2",	"pipe_detected_cam2",	"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"explore",		"follow_wall",		"wall_detected",	"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_obj_cam1",	"follow_pipe_cam1",	"pipe_detected_cam1",	"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_obj_cam2",	"follow_pipe_cam2",	"pipe_detected_cam2",	"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_obj_cam1",	"explore",		"stop_follow",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_obj_cam2",	"explore",		"stop_follow",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_pipe_cam1",	"explore",		"stop_follow",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_pipe_cam2",	"explore",		"stop_follow",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_wall",		"explore",		"stop_follow",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"stay",			"explore",		"begin_explore",	"fsm_unlocked",	"update_fsm",	(void*) this);
	// End

	// Go back to remote chain
	fsm.Add_transition(	"follow_obj_cam1",	"explore",		"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_obj_cam2",	"explore",		"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_pipe_cam1",	"explore",		"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_pipe_cam2",	"explore",		"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"follow_wall",		"explore",		"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"explore",		"stay",			"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"down",			"stay",			"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"up",			"stay",			"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	fsm.Add_transition(	"stay",			"remote",		"go_to_remote",		"fsm_unlocked",	"update_fsm",	(void*) this);
	// End

	fsm_state = REMOTE;
	fsm.Launch("remote");
	drawer.Draw_FSM("FSM_lirmia", &fsm);

}

State_machine::~State_machine(){}

void State_machine::On_start(){}

void State_machine::IO(){
	Link_input("xyz",		COMFLOAT,	3, xyz);
	Link_input("remote",		COMBOOL,	1, &remote);
	Link_input("fsm_unlocked",	COMBOOL,	1, &fsm_unlocked);
	Link_input("fsm_stabilize",	COMBOOL,	1, &fsm_stabilize);
	Link_input("fsm_down",		COMBOOL,	1, &fsm_down);
	Link_input("fsm_up",		COMBOOL,	1, &fsm_up);
	Link_input("fsm_explore",	COMBOOL,	1, &fsm_explore);
	Link_input("fsm_nofollow",	COMBOOL,	1, &fsm_nofollow);
	Link_input("cam_detect_obj",	COMBOOL,	2, cam_detect_obj);
	Link_input("cam_detect_pipe",	COMBOOL,	2, cam_detect_pipe);
	Link_input("cam_detect_opi",	COMBOOL,	1, &cam_detect_opi);

	Link_output("fsm_state",	COMINT,		1, &fsm_state);
}

void State_machine::Job(){
	Critical_receive();
	// TODO : appel de l'événement "wall_detected"
		if(cam_detect_opi){
		switch(fsm_state){
			case EXPLORE:		fsm.Call_event("go_to_autonomous");	break;
			case STAY:		fsm.Call_event("go_up");		break;
			case FOLLOW_OBJ_CAM1:	fsm.Call_event("stop_follow");		break;
			case FOLLOW_OBJ_CAM2:	fsm.Call_event("stop_follow");		break;
			case FOLLOW_PIPE_CAM1:	fsm.Call_event("stop_follow");		break;
			case FOLLOW_PIPE_CAM2:	fsm.Call_event("stop_follow");		break;
			default:							break;
		}
	}
	if(fsm_state != REMOTE && remote){fsm.Call_event("go_to_remote");}
	if(fsm_state == REMOTE && !remote || fsm_stabilize){fsm.Call_event("go_to_autonomous");}
	if(fsm_down){fsm.Call_event("go_down");}
	if(fsm_up){fsm.Call_event("go_up");}
	if(fsm_explore){fsm.Call_event("begin_explore");}
	if(fsm_nofollow){fsm.Call_event("stop_follow");}
	if(fsm_state == EXPLORE){
		if(fsm_up){fsm.Call_event("go_up");}
		if(!fsm_nofollow && !cam_detect_opi){
			if(cam_detect_pipe[0]){fsm.Call_event("pipe_detected_cam1");}
			if(cam_detect_pipe[1]){fsm.Call_event("pipe_detected_cam2");}
			if(cam_detect_obj[0]){fsm.Call_event("found_something_cam1");}
			if(cam_detect_obj[1]){fsm.Call_event("found_something_cam2");}
		}
	}
	if(fsm_state == FOLLOW_OBJ_CAM1){
		if(cam_detect_pipe[0]){fsm.Call_event("pipe_detected_cam1");}
		if(!cam_detect_obj[0] && !cam_detect_pipe[0]){fsm.Call_event("stop_follow");}
	}
	if(fsm_state == FOLLOW_OBJ_CAM2){
		if(cam_detect_pipe[1]){fsm.Call_event("pipe_detected_cam2");}
		if(!cam_detect_obj[1] && !cam_detect_pipe[1]){fsm.Call_event("stop_follow");}
	}
	if(fsm_state == FOLLOW_PIPE_CAM1 && !cam_detect_pipe[0]){fsm.Call_event("stop_follow");}
	if(fsm_state == FOLLOW_PIPE_CAM2 && !cam_detect_pipe[1]){fsm.Call_event("stop_follow");}

	Critical_send();
}

string State_machine::Decode_state_str(int int_state){
	switch(int_state){
		case STAY:		return "stabilize position      ";
		case DOWN:		return "going down              ";
		case EXPLORE:		return "explore                 ";
		case FOLLOW_OBJ_CAM1:	return "follow object camera 1  ";
		case FOLLOW_OBJ_CAM2:	return "follow object camera 2  ";
		case FOLLOW_PIPE_CAM1:	return "follow pipeline camera 1";
		case FOLLOW_PIPE_CAM2:	return "follow pipeline camera 2";
		case FOLLOW_WALL:	return "following wall          ";
		case UP:		return "going up                ";
		case REMOTE:		return "remote control          ";
		default :		return "unknown ???             ";
	}
}

void State_machine::Update_fsm(void* obj){
	State_machine* state_machine = (State_machine*) obj;
	state_machine->fsm_state = state_machine->fsm.Get_current_states_id()[0];
}
