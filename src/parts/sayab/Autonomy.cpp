#include "Autonomy.h"

using namespace std;

Autonomy::Autonomy() : ComThread(){
	motor = 0.;
	rudder = 0.;
	bow_thruster = 0.;
	tzer = false;
	ti = 0.;
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
	Link_input("vthz",			COMFLOAT,	1, &vthz);
	Link_input("yawref",			COMFLOAT,	1, &yawref);
	Link_input("uw",			COMFLOAT,	1, &uw);
	Link_input("uf",			COMFLOAT,	1, &uf);

	Link_output("motor",			COMFLOAT,	1, &motor);
	Link_output("rudder",			COMFLOAT,	1, &rudder);
	Link_output("bow_thruster",		COMFLOAT,	1, &bow_thruster);
	Link_output("ti",			COMFLOAT,	1, &ti);
}

void Autonomy::Job(){
	Critical_receive();
	if(fsm_state == REMOTE){
		tzer = false;
		motor		= remote_forward / 3;
		rudder		= remote_turn;
		bow_thruster	= remote_turn;
	}
	else if(fsm_state == EXPLORE){
		motor 		= 0.1;
		rudder		= 0.;
		bow_thruster	= 0.2;
	}
	else if(fsm_state == STAY){
		motor		= 0.;
		rudder		= 0.;
		bow_thruster	= 0.;
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
	else if(fsm_state == LAW_CONTROL){
		if(tzer == false){ti = t;}
		tzer = true;
		motor		= 0.2;
//		motor		= -0.004*uf;
		rudder		= 0.;
		bow_thruster	= -0.004*uw;

	}
	else{
		motor		= 0.;
		rudder		= 0.;
		bow_thruster	= 0.;
	}
	Critical_send();
}
