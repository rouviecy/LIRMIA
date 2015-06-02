#include "Autonomy.h"

using namespace std;

Autonomy::Autonomy() : ComThread(){
	motor = 0.;
	rudder = 0.;
	bow_thruster = 0.;
}

Autonomy::~Autonomy(){}

void Autonomy::On_start(){}

void Autonomy::IO(){
	Link_input("fsm_state",			COMINT,		1, &fsm_state);
	Link_input("remote_forward",		COMFLOAT,	1, &remote_forward);
	Link_input("remote_turn",		COMFLOAT,	1, &remote_turn);
	Link_input("cam_detect_size",		COMFLOAT,	2, cam_detect_size);
	Link_input("cam_detect_horizontal",	COMFLOAT,	2, cam_detect_horizontal);
	Link_input("cam_detect_vertical",	COMFLOAT,	2, cam_detect_vertical);
	Link_input("t",				COMFLOAT,	1, &t);
	Link_input("xy",			COMFLOAT,	2, xy);
	Link_input("thz",			COMFLOAT,	1, &thz);

	Link_output("motor",			COMFLOAT,	1, &motor);
	Link_output("rudder",			COMFLOAT,	1, &rudder);
	Link_output("bow_thruster",		COMFLOAT,	1, &bow_thruster);
}

void Autonomy::Job(){
	Critical_receive();
	if(fsm_state == REMOTE){
		motor		= remote_forward;
		rudder		= remote_turn;
		bow_thruster	= remote_turn;
	}
	else if(fsm_state == FOLLOW_CAM_SUB){
		motor		= -cam_detect_vertical[1];
		rudder		= -cam_detect_horizontal[1];
		bow_thruster	= -cam_detect_horizontal[1];
	}
	else if(fsm_state == FOLLOW_CAM_SURF){
		if(cam_detect_size[0] < 0.20){
			motor		= -0.2;
			rudder		= -cam_detect_horizontal[0];
			bow_thruster	= -cam_detect_horizontal[0];
		}
		else{
			motor		= 0.2 - cam_detect_size[0];
			rudder		= -cam_detect_horizontal[0];
			bow_thruster	= -cam_detect_horizontal[0];
		}
	}
	else if(fsm_state == LOST_CAM_SUB){
		motor		= 0.2;
		rudder		= 0.;
		bow_thruster	= 0.;
	}
	else if(fsm_state == LOST_CAM_SURF){
		motor		= 0.;
		rudder		= 0.;
		bow_thruster	= 0.2;
	}
	else{
		motor		= 0.;
		rudder		= 0.;
		bow_thruster	= 0.;
	}
	Critical_send();
}
