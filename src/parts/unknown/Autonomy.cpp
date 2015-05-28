#include "Autonomy.h"

using namespace std;

Autonomy::Autonomy() : ComThread(){
	motor = 0.;
	rudder = 0.;
	bow_thrusters[0] = 0.;
	bow_thrusters[1] = 0.;
}

Autonomy::~Autonomy(){}

void Autonomy::On_start(){}

void Autonomy::IO(){
	Link_input("fsm_state",			COMINT,		1, &fsm_state);
	Link_input("remote_forward",		COMFLOAT,	1, &remote_forward);
	Link_input("remote_turn",		COMFLOAT,	1, &remote_turn);
	Link_input("remote_lateral",		COMFLOAT,	1, &remote_lateral);
	Link_input("cam_detect_horizontal",	COMFLOAT,	1, &cam_detect_horizontal);
	Link_input("cam_detect_vertical",	COMFLOAT,	1, &cam_detect_vertical);
	Link_input("t",				COMFLOAT,	1, &t);
	Link_input("xy",			COMFLOAT,	2, xy);
	Link_input("thz",			COMFLOAT,	1, &thz);

	Link_output("motor",			COMFLOAT,	1, &motor);
	Link_output("rudder",			COMFLOAT,	1, &rudder);
	Link_output("bow_thrusters",		COMFLOAT,	2, bow_thrusters);
}

void Autonomy::Job(){
	Critical_receive();
	if(fsm_state == REMOTE){// TODO : check
		motor			= remote_forward;
		rudder			= remote_turn;
		bow_thrusters[0]	= +remote_lateral;
		bow_thrusters[1]	= -remote_lateral;
	}
	else if(fsm_state == FOLLOW_OBJ_CAM){// TODO : check
		motor			= -cam_detect_vertical;
		rudder			= -cam_detect_horizontal;
		bow_thrusters[0]	= +cam_detect_horizontal;
		bow_thrusters[1]	= -cam_detect_horizontal;
	}
	else{
		motor			= 0.;
		rudder			= 0.;
		bow_thrusters[0]	= 0.;
		bow_thrusters[1]	= 0.;
	}
	Critical_send();
}
