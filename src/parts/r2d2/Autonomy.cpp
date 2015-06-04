#include "Autonomy.h"

using namespace std;

Autonomy::Autonomy() : ComThread(){
	for(int i = 0; i < 4; i++){motor[i] = 0.;}
}

Autonomy::~Autonomy(){}

void Autonomy::On_start(){}

void Autonomy::IO(){
	Link_input("remote_forward",		COMFLOAT,	1, &remote_forward);
	Link_input("remote_turn",		COMFLOAT,	1, &remote_turn);
	Link_input("remote_deeper",		COMFLOAT,	1, &remote_deeper);
	Link_input("t",				COMFLOAT,	1, &t);
	Link_input("cam_size_obj",		COMFLOAT,	1, &cam_size_obj);
	Link_input("cam_detect_horizontal",	COMFLOAT,	1, &cam_detect_horizontal);
	Link_input("cam_detect_vertical",	COMFLOAT,	1, &cam_detect_vertical);
	Link_input("cam_pipeline_angle",	COMFLOAT,	1, &cam_pipeline_angle);
	Link_input("cam_pipeline_distance",	COMFLOAT,	1, &cam_pipeline_distance);

	Link_output("motor",			COMFLOAT,	4, motor);
}

void Autonomy::Job(){
	Critical_receive();
	if(fsm_state == FOLLOW_OBJ_CAM){
		if(cam_size_obj < 0.20){
			motor[0] = 0.2 - cam_size_obj + cam_detect_horizontal / 3;
			motor[1] = 0.2 - cam_size_obj - cam_detect_horizontal / 3;
			motor[2] = -cam_detect_vertical / 3;
			motor[3] = -cam_detect_vertical / 3;
		}
		else{
			motor[0] = -0.2;
			motor[1] = -0.2;
			motor[2] = 0.;
			motor[3] = 0.;
		}
	}
	else if(fsm_state == FOLLOW_PIPE_CAM){
		if(cam_size_obj < 0.20){
			motor[0] = 0.2 - cam_size_obj + cam_pipeline_distance / 3;
			motor[1] = 0.2 - cam_size_obj - cam_pipeline_distance / 3;
			motor[2] = 0.2;
			motor[3] = 0.2;
		}
		else{
			motor[0] = -0.2;
			motor[1] = -0.2;
			motor[2] = 0.;
			motor[3] = 0.;
		}
	}
	else if(fsm_state == FOLLOW_WALL){
		motor[0] = 0.;
		motor[1] = 0.;
		motor[2] = 0.;
		motor[3] = 0.;
	}
	else if(fsm_state == REMOTE){
//		keep_thz = thxyz[3];
		motor[0] = remote_forward / 2 + remote_turn / 2;
		motor[1] = remote_forward / 2 - remote_turn / 2;
		motor[2] = -remote_deeper / 2;
		motor[3] = -remote_deeper / 2;
	}
	else if(fsm_state == EXPLORE){
//		keep_thz = thxyz[3];
		motor[0] = +0.5;
		motor[1] = +0.5;
		motor[2] = 0.;
		motor[3] = 0.;
	}
	else if(fsm_state == UP || fsm_state == DOWN || fsm_state == STAY){ // TODO : check
//		float diff_thz = keep_thz - thxyz[3];
		float diff_thz = 42.;
		if(diff_thz < -360){diff_thz += 360;}
		if(diff_thz > +360){diff_thz -= 360;}
		if(diff_thz < -180){diff_thz += 360;}
		if(diff_thz > +180){diff_thz -= 360;}
		motor[0] = - diff_thz / 500;
		motor[1] = + diff_thz / 500;
		if(fsm_state == STAY){
//			motor[2] = + thxyz[2] / 500;
//			motor[3] = - thxyz[2] / 500;
			motor[2] = 0.;
			motor[3] = 0.;
		}
		else{
//			motor[2] = + thxyz[2] / 500 - remote_deeper / 2;
//			motor[3] = - thxyz[2] / 500 - remote_deeper / 2;
			motor[2] = 0.;
			motor[3] = 0.;
		}
	}
	else{
		motor[0] = 0.;
		motor[1] = 0.;
		motor[2] = 0.;
		motor[3] = 0.;
	}
	Critical_send();
}
