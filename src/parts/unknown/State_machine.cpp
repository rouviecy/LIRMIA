#include "State_machine.h"

using namespace std;

State_machine::State_machine() : ComThread(){

	fsm.Add_state("remote",			REMOTE);
	fsm.Add_state("follow_obj_cam",		FOLLOW_OBJ_CAM);
	fsm.Add_state("lost_obj_cam",		LOST_OBJ_CAM);

	fsm.Add_event("go_to_autonomous");
	fsm.Add_event("go_to_remote");
	fsm.Add_event("detected_obj");
	fsm.Add_event("lost_obj");

				// state_from		// state_to		// trigger		// guard	// action	// object
	fsm.Add_transition(	"remote",		"follow_obj_cam",	"go_to_autonomous",	"",		"",		NULL);
	fsm.Add_transition(	"lost_obj_cam",		"follow_obj_cam",	"detected_obj",		"",		"",		NULL);
	fsm.Add_transition(	"follow_obj_cam",	"lost_obj_cam",		"lost_obj",		"",		"",		NULL);
	fsm.Add_transition(	"lost_obj_cam",		"remote",		"go_to_remote",		"",		"",		NULL);
	fsm.Add_transition(	"follow_obj_cam",	"remote",		"go_to_remote",		"",		"",		NULL);

	fsm_state = REMOTE;
	fsm.Launch("remote");
	drawer.Draw_FSM("FSM_unknown", &fsm);

}

State_machine::~State_machine(){}

void State_machine::On_start(){}

void State_machine::IO(){
	Link_input("remote",		COMBOOL,	1, &remote);
	Link_input("cam_detect_obj",	COMBOOL,	1, &cam_detect_obj);

	Link_output("fsm_state",	COMINT,		1, &fsm_state);
}

void State_machine::Job(){
	Critical_receive();
	if(fsm_state == REMOTE && !remote){
		fsm_state = FOLLOW_OBJ_CAM;
		fsm.Call_event("go_to_autonomous");
	}
	if(fsm_state != REMOTE && remote){
		fsm_state = REMOTE;
		fsm.Call_event("go_to_remote");
	}
	if(fsm_state == FOLLOW_OBJ_CAM && !cam_detect_obj){
		fsm_state = LOST_OBJ_CAM;
		fsm.Call_event("lost_obj");
	}
	if(fsm_state == LOST_OBJ_CAM && cam_detect_obj){
		fsm_state = FOLLOW_OBJ_CAM;
		fsm.Call_event("detected_obj");
	}
	Critical_send();
}

string State_machine::Decode_state_str(int int_state){
	switch(int_state){
		case FOLLOW_OBJ_CAM:	return "follow object camera";
		case LOST_OBJ_CAM:	return "lost object camera  ";
		case REMOTE:		return "remote control      ";
		default :		return "unknown ???         ";
	}
}
