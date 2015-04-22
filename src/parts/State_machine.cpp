#include "State_machine.h"

using namespace std;

State_machine::State_machine() : ComThread(){

	fsm_state = 0.;
	my_guard = true;

	fsm.Add_state("remote");
	fsm.Add_state("stay");
	fsm.Add_state("down");
	fsm.Add_state("explore");
	fsm.Add_state("follow_cam1");
	fsm.Add_state("follow_cam2");
	fsm.Add_state("up");

	fsm.Add_event("go_to_autonomous");
	fsm.Add_event("go_down");
	fsm.Add_event("begin_explore");
	fsm.Add_event("found_something_cam1");
	fsm.Add_event("found_something_cam2");
	fsm.Add_event("stop_follow");
	fsm.Add_event("go_up");
	fsm.Add_event("reach_surface");
	fsm.Add_event("go_to_remote");

	fsm.Add_action("act_remote_to_stay",		&(State_machine::Act_remote_to_stay));
	fsm.Add_action("act_stay_to_down",		&(State_machine::Act_stay_to_down));
	fsm.Add_action("act_down_to_explore",		&(State_machine::Act_down_to_explore));
	fsm.Add_action("act_explore_to_follow_cam1",	&(State_machine::Act_explore_to_follow_cam1));
	fsm.Add_action("act_explore_to_follow_cam2",	&(State_machine::Act_explore_to_follow_cam2));
	fsm.Add_action("act_follow_to_explore",		&(State_machine::Act_follow_to_explore));
	fsm.Add_action("act_explore_to_up",		&(State_machine::Act_explore_to_up));
	fsm.Add_action("act_up_to_stay",		&(State_machine::Act_up_to_stay));
	fsm.Add_action("act_stay_to_remote",		&(State_machine::Act_stay_to_remote));

	fsm.Add_guard("guard", &my_guard);

				// state_from	// state_to	// trigger		// guard	// action			// object
	fsm.Add_transition(	"remote",	"stay",		"go_to_autonomous",	"guard",	"act_remote_to_stay",		(void*) this);
	fsm.Add_transition(	"stay",		"down",		"go_down",		"guard",	"act_stay_to_down",		(void*) this);
	fsm.Add_transition(	"down",		"explore",	"begin_explore",	"guard",	"act_down_to_explore",		(void*) this);
	fsm.Add_transition(	"explore",	"follow_cam1",	"found_something_cam1",	"guard",	"act_explore_to_follow_cam1",	(void*) this);
	fsm.Add_transition(	"explore",	"follow_cam2",	"found_something_cam2",	"guard",	"act_explore_to_follow_cam2",	(void*) this);
	fsm.Add_transition(	"follow_cam1",	"explore",	"stop_follow",		"guard",	"act_follow_to_explore",	(void*) this);
	fsm.Add_transition(	"follow_cam2",	"explore",	"stop_follow",		"guard",	"act_follow_to_explore",	(void*) this);
	fsm.Add_transition(	"explore",	"up",		"go_up",		"guard",	"act_explore_to_up",		(void*) this);
	fsm.Add_transition(	"up",		"stay",		"reach_surface",	"guard",	"act_up_to_stay",		(void*) this);
	fsm.Add_transition(	"stay",		"remote",	"go_to_remote",		"guard",	"act_stay_to_remote",		(void*) this);

	fsm.Launch("explore");
	current_state = EXPLORE;
	drawer.Draw_FSM("FSM", &fsm);

}

State_machine::~State_machine(){}

void State_machine::On_start(){}

void State_machine::IO(){
	Link_input("z", &z);
	Link_input("remote", &remote);
	Link_input("cam_detect1", &cam_detect1);
	Link_input("cam_detect2", &cam_detect2);
	Link_output("fsm_state", &fsm_state);
}

void State_machine::Job(){
	Critical_receive();
	if(cam_detect1 > 0){
		fsm.Call_event("found_something_cam1");
	}
	if(cam_detect2 > 0){
		fsm.Call_event("found_something_cam2");
	}
	if(cam_detect1 < 0 && cam_detect2 < 0){
		fsm.Call_event("stop_follow");
	}
	Critical_send();
}

state_t State_machine::Decode_state(float float_state){
	if	(float_state < 0.5)	{return STAY;}
	else if	(float_state < 1.5)	{return DOWN;}
	else if	(float_state < 2.5)	{return EXPLORE;}
	else if	(float_state < 3.5)	{return FOLLOW_CAM1;}
	else if	(float_state < 4.5)	{return FOLLOW_CAM2;}
	else if	(float_state < 5.5)	{return UP;}
	else if	(float_state < 6.5)	{return REMOTE;}
	else				{return UNKNOWN;}
}

void State_machine::Act_remote_to_stay(void* obj){
	((State_machine*) obj)->fsm_state = 0.;
}
void State_machine::Act_stay_to_down(void* obj){
	((State_machine*) obj)->fsm_state = 1.;
}
void State_machine::Act_down_to_explore(void* obj){
	((State_machine*) obj)->fsm_state = 2.;
}
void State_machine::Act_explore_to_follow_cam1(void* obj){
	((State_machine*) obj)->fsm_state = 3.;
}
void State_machine::Act_explore_to_follow_cam2(void* obj){
	((State_machine*) obj)->fsm_state = 4.;
}
void State_machine::Act_follow_to_explore(void* obj){
	((State_machine*) obj)->fsm_state = 2.;
}
void State_machine::Act_explore_to_up(void* obj){
	((State_machine*) obj)->fsm_state = 5.;
}
void State_machine::Act_up_to_stay(void* obj){
	((State_machine*) obj)->fsm_state = 0.;
}
void State_machine::Act_stay_to_remote(void* obj){
	((State_machine*) obj)->fsm_state = 6.;
}
