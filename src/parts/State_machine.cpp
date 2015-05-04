#include "State_machine.h"

using namespace std;

State_machine::State_machine() : ComThread(){

	fsm_state = 8.;

	fsm.Add_state("remote");
	fsm.Add_state("stay");
	fsm.Add_state("down");
	fsm.Add_state("explore");
	fsm.Add_state("follow_obj_cam1");
	fsm.Add_state("follow_obj_cam2");
	fsm.Add_state("follow_pipe_cam1");
	fsm.Add_state("follow_pipe_cam2");
	fsm.Add_state("up");

	fsm.Add_event("go_to_autonomous");
	fsm.Add_event("go_down");
	fsm.Add_event("begin_explore");
	fsm.Add_event("found_something_cam1");
	fsm.Add_event("found_something_cam2");
	fsm.Add_event("pipe_detected_cam1");
	fsm.Add_event("pipe_detected_cam2");
	fsm.Add_event("stop_follow");
	fsm.Add_event("go_up");
	fsm.Add_event("reach_surface");
	fsm.Add_event("go_to_remote");

	fsm.Add_action("act_to_stay",			&(State_machine::Act_to_stay));
	fsm.Add_action("act_to_down",			&(State_machine::Act_to_down));
	fsm.Add_action("act_to_explore",		&(State_machine::Act_to_explore));
	fsm.Add_action("act_to_follow_obj_cam1",	&(State_machine::Act_to_follow_obj_cam1));
	fsm.Add_action("act_to_follow_obj_cam2",	&(State_machine::Act_to_follow_obj_cam2));
	fsm.Add_action("act_to_follow_pipe_cam1",	&(State_machine::Act_to_follow_pipe_cam1));
	fsm.Add_action("act_to_follow_pipe_cam2",	&(State_machine::Act_to_follow_pipe_cam2));
	fsm.Add_action("act_to_up",			&(State_machine::Act_to_up));
	fsm.Add_action("act_to_remote",			&(State_machine::Act_to_remote));

	fsm.Add_guard("fsm_unlocked", &guard_unlocked);

				// state_from		// state_to		// trigger		// guard	// action			// object
	// Go to stabilization
	fsm.Add_transition(	"remote",		"stay",			"go_to_autonomous",	"fsm_unlocked",	"act_to_stay",			(void*) this);
	fsm.Add_transition(	"down",			"stay",			"go_to_autonomous",	"fsm_unlocked",	"act_to_stay",			(void*) this);
	fsm.Add_transition(	"up",			"stay",			"go_to_autonomous",	"fsm_unlocked",	"act_to_stay",			(void*) this);
	fsm.Add_transition(	"explore",		"stay",			"go_to_autonomous",	"fsm_unlocked",	"act_to_stay",			(void*) this);
	// End

	// Go up and down
	fsm.Add_transition(	"stay",			"down",			"go_down",		"fsm_unlocked",	"act_to_down",			(void*) this);
	fsm.Add_transition(	"stay",			"up",			"go_up",		"fsm_unlocked",	"act_to_up",			(void*) this);
	// End

	// Between explore and follow
	fsm.Add_transition(	"explore",		"follow_obj_cam1",	"found_something_cam1",	"fsm_unlocked",	"act_to_follow_obj_cam1",	(void*) this);
	fsm.Add_transition(	"explore",		"follow_obj_cam2",	"found_something_cam2",	"fsm_unlocked",	"act_to_follow_obj_cam2",	(void*) this);
	fsm.Add_transition(	"explore",		"follow_pipe_cam1",	"pipe_detected_cam1",	"fsm_unlocked",	"act_to_follow_pipe_cam1",	(void*) this);
	fsm.Add_transition(	"explore",		"follow_pipe_cam2",	"pipe_detected_cam2",	"fsm_unlocked",	"act_to_follow_pipe_cam2",	(void*) this);
	fsm.Add_transition(	"follow_obj_cam1",	"follow_pipe_cam1",	"pipe_detected_cam1",	"fsm_unlocked",	"act_to_follow_pipe_cam1",	(void*) this);
	fsm.Add_transition(	"follow_obj_cam2",	"follow_pipe_cam2",	"pipe_detected_cam2",	"fsm_unlocked",	"act_to_follow_pipe_cam2",	(void*) this);
	fsm.Add_transition(	"follow_obj_cam1",	"explore",		"stop_follow",		"fsm_unlocked",	"act_to_explore",		(void*) this);
	fsm.Add_transition(	"follow_obj_cam2",	"explore",		"stop_follow",		"fsm_unlocked",	"act_to_explore",		(void*) this);
	fsm.Add_transition(	"follow_pipe_cam1",	"explore",		"stop_follow",		"fsm_unlocked",	"act_to_explore",		(void*) this);
	fsm.Add_transition(	"follow_pipe_cam2",	"explore",		"stop_follow",		"fsm_unlocked",	"act_to_explore",		(void*) this);
	fsm.Add_transition(	"stay",			"explore",		"begin_explore",	"fsm_unlocked",	"act_to_explore",		(void*) this);
	// End

	// Go back to remote chain
	fsm.Add_transition(	"follow_obj_cam1",	"explore",		"go_to_remote",		"fsm_unlocked",	"act_to_explore",		(void*) this);
	fsm.Add_transition(	"follow_obj_cam2",	"explore",		"go_to_remote",		"fsm_unlocked",	"act_to_explore",		(void*) this);
	fsm.Add_transition(	"follow_pipe_cam1",	"explore",		"go_to_remote",		"fsm_unlocked",	"act_to_explore",		(void*) this);
	fsm.Add_transition(	"follow_pipe_cam2",	"explore",		"go_to_remote",		"fsm_unlocked",	"act_to_explore",		(void*) this);
	fsm.Add_transition(	"explore",		"stay",			"go_to_remote",		"fsm_unlocked",	"act_to_stay",			(void*) this);
	fsm.Add_transition(	"down",			"stay",			"go_to_remote",		"fsm_unlocked",	"act_to_stay",			(void*) this);
	fsm.Add_transition(	"up",			"stay",			"go_to_remote",		"fsm_unlocked",	"act_to_stay",			(void*) this);
	fsm.Add_transition(	"stay",			"remote",		"go_to_remote",		"fsm_unlocked",	"act_to_remote",		(void*) this);
	// End

	fsm.Launch("remote");
	current_state = EXPLORE;
	drawer.Draw_FSM("FSM", &fsm);

}

State_machine::~State_machine(){}

void State_machine::On_start(){}

void State_machine::IO(){
	Link_input("z", &z);
	Link_input("remote", &remote);
	Link_input("fsm_unlocked", &fsm_unlocked);		Link_input("fsm_stabilize", &fsm_stabilize);
	Link_input("fsm_down", &fsm_down);			Link_input("fsm_up", &fsm_up);
	Link_input("fsm_explore", &fsm_explore);		Link_input("fsm_nofollow", &fsm_nofollow);
	Link_input("cam_detect_obj1", &cam_detect_obj1);	Link_input("cam_detect_pipe1", &cam_detect_pipe1);
	Link_input("cam_detect_obj2", &cam_detect_obj2);	Link_input("cam_detect_pipe2", &cam_detect_pipe2);
	Link_output("fsm_state", &fsm_state);
}

void State_machine::Job(){
	Critical_receive();
	guard_unlocked = (fsm_unlocked > 0);
	if(Decode_state(fsm_state) != REMOTE && remote > 0){fsm.Call_event("go_to_remote");}
	if(Decode_state(fsm_state) == REMOTE && remote < 0 || fsm_stabilize > 0){fsm.Call_event("go_to_autonomous");}
	if(fsm_down > 0){fsm.Call_event("go_down");}
	if(fsm_up > 0){fsm.Call_event("go_up");}
	if(fsm_explore > 0){fsm.Call_event("begin_explore");}
	if(fsm_nofollow > 0){fsm.Call_event("stop_follow");}
	if(Decode_state(fsm_state) == EXPLORE){
		if(fsm_up > 0){fsm.Call_event("go_up");}
		if(fsm_nofollow < 0){
			if(cam_detect_pipe1 > 0){fsm.Call_event("pipe_detected_cam1");}
			if(cam_detect_pipe2 > 0){fsm.Call_event("pipe_detected_cam2");}
			if(cam_detect_obj1 > 0){fsm.Call_event("found_something_cam1");}
			if(cam_detect_obj2 > 0){fsm.Call_event("found_something_cam2");}
		}
	}
	if(Decode_state(fsm_state) == FOLLOW_OBJ_CAM1){
		if(cam_detect_pipe1 > 0){fsm.Call_event("pipe_detected_cam1");}
		if(cam_detect_obj1 < 0 && cam_detect_pipe1 < 0){fsm.Call_event("stop_follow");}
	}
	if(Decode_state(fsm_state) == FOLLOW_OBJ_CAM2){
		if(cam_detect_pipe2 > 0){fsm.Call_event("pipe_detected_cam2");}
		if(cam_detect_obj2 < 0 && cam_detect_pipe2 < 0){fsm.Call_event("stop_follow");}
	}
	if(Decode_state(fsm_state) == FOLLOW_PIPE_CAM1 && cam_detect_pipe1 < 0){fsm.Call_event("stop_follow");}
	if(Decode_state(fsm_state) == FOLLOW_PIPE_CAM2 && cam_detect_pipe2 < 0){fsm.Call_event("stop_follow");}
	Critical_send();
}

state_t State_machine::Decode_state(float float_state){
	// TODO : intégrer dans "interface"
	if	(float_state < 0.5)	{return STAY;}
	else if	(float_state < 1.5)	{return DOWN;}
	else if	(float_state < 2.5)	{return EXPLORE;}
	else if	(float_state < 3.5)	{return FOLLOW_OBJ_CAM1;}
	else if	(float_state < 4.5)	{return FOLLOW_OBJ_CAM2;}
	else if	(float_state < 5.5)	{return FOLLOW_PIPE_CAM1;}
	else if	(float_state < 6.5)	{return FOLLOW_PIPE_CAM2;}
	else if	(float_state < 7.5)	{return UP;}
	else if	(float_state < 8.5)	{return REMOTE;}
	else				{return UNKNOWN;}
}

string State_machine::Decode_state_str(float float_state){
	// TODO : intégrer dans "interface"
	if	(float_state < 0.5)	{return "stabilize position ";}
	else if	(float_state < 1.5)	{return "going down";}
	else if	(float_state < 2.5)	{return "explore";}
	else if	(float_state < 3.5)	{return "follow object camera 1";}
	else if	(float_state < 4.5)	{return "follow object camera 2";}
	else if	(float_state < 5.5)	{return "follow pipeline camera 1";}
	else if	(float_state < 6.5)	{return "follow pipeline camera 2";}
	else if	(float_state < 7.5)	{return "going up";}
	else if	(float_state < 8.5)	{return "remote control";}
	else				{return "unknown ???";}
}

void State_machine::Act_to_stay(void* obj)		{((State_machine*) obj)->fsm_state = 0.;}
void State_machine::Act_to_down(void* obj)		{((State_machine*) obj)->fsm_state = 1.;}
void State_machine::Act_to_explore(void* obj)		{((State_machine*) obj)->fsm_state = 2.;}
void State_machine::Act_to_follow_obj_cam1(void* obj)	{((State_machine*) obj)->fsm_state = 3.;}
void State_machine::Act_to_follow_obj_cam2(void* obj)	{((State_machine*) obj)->fsm_state = 4.;}
void State_machine::Act_to_follow_pipe_cam1(void* obj)	{((State_machine*) obj)->fsm_state = 5.;}
void State_machine::Act_to_follow_pipe_cam2(void* obj)	{((State_machine*) obj)->fsm_state = 6.;}
void State_machine::Act_to_up(void* obj)		{((State_machine*) obj)->fsm_state = 7.;}
void State_machine::Act_to_remote(void* obj)		{((State_machine*) obj)->fsm_state = 8.;}
