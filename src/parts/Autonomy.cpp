#include "Autonomy.h"

using namespace std;

Autonomy::Autonomy() : ComThread(){
	motor1 = 0.;
	motor2 = 0.;
	motor3 = 0.;
	motor4 = 0.;
}

Autonomy::~Autonomy(){}

void Autonomy::On_start(){}

void Autonomy::IO(){
	Link_input("fsm_state", &fsm_state);
	Link_input("remote_forward", &remote_forward);
	Link_input("remote_turn", &remote_turn);
	Link_input("remote_deeper", &remote_deeper);
	Link_input("t", &t);
	Link_input("x", &x);        Link_input("y", &y);        Link_input("z", &z);
	Link_input("vx", &vx);      Link_input("vy", &vy);      Link_input("vz", &vz);
	Link_input("thx", &thx);    Link_input("thy", &thy);    Link_input("thz", &thz);
	Link_input("vthx", &vthx);  Link_input("vthy", &vthy);  Link_input("vthz", &vthz);
	Link_input("obst_x", &x);   Link_input("obst_y", &y);
	Link_input("obst_vx", &vx); Link_input("obst_vy", &vy);
	Link_input("cam_detect1_horizontal", &cam_detect1_horizontal);	Link_input("cam_detect1_vertical", &cam_detect1_vertical);
	Link_input("cam_detect2_horizontal", &cam_detect2_horizontal);	Link_input("cam_detect2_vertical", &cam_detect2_vertical);
	Link_input("pipeline_angle_cam1", &pipeline_angle_cam1);	Link_input("pipeline_distance_cam1", &pipeline_distance_cam1);
	Link_input("pipeline_angle_cam2", &pipeline_angle_cam2);	Link_input("pipeline_distance_cam2", &pipeline_distance_cam2);

	Link_output("motor1", &motor1);
	Link_output("motor2", &motor2);
	Link_output("motor3", &motor3);
	Link_output("motor4", &motor4);
}

void Autonomy::Job(){
	Critical_receive();
	state_t current_state = State_machine::Decode_state(fsm_state);
	if(current_state == FOLLOW_OBJ_CAM1){
		motor1 = +cam_detect1_horizontal; // TODO : régler sens
		motor2 = -cam_detect1_horizontal;
		motor3 = +cam_detect1_vertical;
		motor4 = +cam_detect1_vertical;
	}
	else if(current_state == FOLLOW_OBJ_CAM2){ // TODO : régler sens
		motor1 = +cam_detect1_horizontal;
		motor2 = -cam_detect1_horizontal;
		motor3 = +cam_detect1_vertical;
		motor4 = +cam_detect1_vertical;
	}
	else if(current_state == FOLLOW_PIPE_CAM1){
		// TODO
	}
	else if(current_state == FOLLOW_PIPE_CAM2){
		// TODO
	}
	else if(current_state == REMOTE){ // TODO : régler sens
		motor1 = remote_forward / 2 - remote_turn / 2;
		motor2 = remote_forward / 2 + remote_turn / 2;
		motor3 = - remote_deeper / 2;
		motor4 = - remote_deeper / 2;
	}
	else{
		motor1 = 0.;
		motor2 = 0.;
		motor3 = 0.;
		motor4 = 0.;
	}
	Critical_send();
}
