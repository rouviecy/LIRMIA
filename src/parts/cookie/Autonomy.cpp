#include "Autonomy.h"
#define saturation(minimum, value, maximum) (value > maximum ? maximum : (value < minimum ? minimum : value))

using namespace std;

Autonomy::Autonomy() : ComThread(){
	for(int i = 0; i < 4; i++){motor[i] = 0.;}
	last_cam_detect_horizontal = 0.;
	last_cam_detect_vertical = 0.;
	last_t = -1.;
}

Autonomy::~Autonomy(){}

void Autonomy::On_start(){}

void Autonomy::IO(){
	Link_input("fsm_state",			COMINT,		1, &fsm_state);
	Link_input("remote_forward",		COMFLOAT,	1, &remote_forward);
	Link_input("remote_turn",		COMFLOAT,	1, &remote_turn);
	Link_input("remote_deeper",		COMFLOAT,	1, &remote_deeper);
	Link_input("t",				COMFLOAT,	1, &t);
	Link_input("thxyz",			COMFLOAT,	3, thxyz);
	Link_input("cam_size_obj",		COMFLOAT,	1, &cam_size_obj);
	Link_input("cam_detect_horizontal",	COMFLOAT,	1, &cam_detect_horizontal);
	Link_input("cam_detect_vertical",	COMFLOAT,	1, &cam_detect_vertical);
	Link_input("vthxyz",			COMFLOAT,	3, vthxyz);

	Link_output("motor",			COMFLOAT,	4, motor);
}

void Autonomy::Job(){
	Critical_receive();
	if(fsm_state == FOLLOW_OBJ_CAM){
		if(cam_size_obj < 0.20){
			float v_horizontal = (cam_detect_horizontal - last_cam_detect_horizontal) / (t - last_t);
			float v_vertical = (cam_detect_vertical - last_cam_detect_vertical) / (t - last_t);
			motor[1] = 0.2 - cam_size_obj + cam_detect_horizontal / 20;
			motor[2] = 0.2 - cam_size_obj - cam_detect_horizontal / 20;
			motor[0] = -cam_detect_vertical / 20;
			motor[3] = -cam_detect_vertical / 20;
			last_cam_detect_horizontal = cam_detect_horizontal;
			last_cam_detect_vertical = cam_detect_vertical;
			last_t = t;
		}
		else{
			motor[1] = -0.05;
			motor[2] = -0.05;
			motor[0] = 0.;
			motor[3] = 0.;
		}
	}
	else if(fsm_state == FOLLOW_WALL){
		motor[1] = 0.;
		motor[2] = 0.;
		motor[0] = 0.;
		motor[3] = 0.;
	}
	else if(fsm_state == REMOTE){
//		keep_thz = thxyz[3];
		motor[1] = - (remote_forward / 2 + remote_turn / 2);
		motor[2] = remote_turn / 2 - remote_forward / 2;
		motor[0] = -remote_deeper / 2;
		motor[3] = -remote_deeper / 2;
	}
	else if(fsm_state == EXPLORE){
//		keep_thz = thxyz[3];
		motor[1] = 0.;
		motor[2] = 0.;
		motor[0] = 0.;
		motor[3] = 0.;
	}
	else if(fsm_state == UP || fsm_state == DOWN || fsm_state == STAY){ // TODO : check
		
		//---------------------------YAW CONTROL --------------------------------------
		
		float A=thxyz[2];
		
				
		float difA,RF=280,CTRL,DCTRL,AVAN=0, inte, ctrlI;
		float kp=0.01, kd=0.005, ki=0.7, CP=0,CD=0; 
		inte=0;
				
        // cout << A << "   " << RF << endl;
		//---------------------------------------------------------------------------------
				
			difA=A-RF;

			if (difA < 180)
			{
			difA = -180 -difA;
			}
			else if (difA > 180)
			{
			difA = 180-difA;
			}
			CP=kp*(difA);
			CP=saturation(-1,CP,1);

			//CD=kd*(vthxyz[2]);
			//CD=saturation(-1,CD,1);
			
			//inte=inte+(difA);
			//ctrlI= ki*inte;

			CTRL=(CP+CD);
			//CTRL=saturation(-1,CTRL,1);

			motor[1] = +CTRL;
			motor[2] = -CTRL;
	cout << A << "   " << RF << "  " << CTRL << "  " <<  vthxyz[2] << endl;	
		
	}
	else{
		motor[1] = 0.;
		motor[2] = 0.;
		motor[0] = 0.;
		motor[3] = 0.;
	}
	Critical_send();
}
