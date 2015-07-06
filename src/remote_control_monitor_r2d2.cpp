#include "interfaces/Joystick.h"
#include "interfaces/vision/HSV_tools.h"
#include "parts/r2d2/State_machine.h"
#include <iostream>
#include <vector>

using namespace std;

#define MONITOR_SIZE		500
#define MONITOR_BORDER		50
#define DELTA_SIZE_BORDER	450.0

typedef struct{
	TCP_client* tcp_client;
	bool go_on;
	bool remote_mode;
	bool unlocked;
	bool enable_streaming;
} struct_callback;

typedef struct{
	float t, x, y, z, thz;
	int motor1, motor2, motor3, motor4;
	int state;
	bool unlocked;
	float min_coord, max_coord;
	vector <vector <float> > path;
	vector <vector <float> > objects;
} struct_monitor;
	

static void send_move_order(void* obj, string msg){
	struct_callback* obj_callback = (struct_callback*) obj;
	obj_callback->tcp_client->Send(msg);
}

static void listen_key_up	(void* obj, bool down){send_move_order(obj, "ra" + to_string(down));}
static void listen_key_down	(void* obj, bool down){send_move_order(obj, "rb" + to_string(down));}
static void listen_key_left	(void* obj, bool down){send_move_order(obj, "rl" + to_string(down));}
static void listen_key_right	(void* obj, bool down){send_move_order(obj, "rr" + to_string(down));}
static void listen_key_plus	(void* obj, bool down){send_move_order(obj, "ru" + to_string(down));}
static void listen_key_minus	(void* obj, bool down){send_move_order(obj, "rd" + to_string(down));}

static void listen_key_s	(void* obj, bool down){send_move_order(obj, "fs" + to_string(down));}
static void listen_key_i	(void* obj, bool down){send_move_order(obj, "fu" + to_string(down));}
static void listen_key_k	(void* obj, bool down){send_move_order(obj, "fd" + to_string(down));}
static void listen_key_e	(void* obj, bool down){send_move_order(obj, "fe" + to_string(down));}
static void listen_key_n	(void* obj, bool down){send_move_order(obj, "fn" + to_string(down));}

static void listen_key_r(void* obj, bool down){
	if(down){
		struct_callback* obj_callback = (struct_callback*) obj;
		obj_callback->remote_mode = !(obj_callback->remote_mode);
		obj_callback->tcp_client->Send(obj_callback->remote_mode ? "r1" : "r0");
	}
}

static void listen_key_l(void* obj, bool down){
	if(down){
		struct_callback* obj_callback = (struct_callback*) obj;
		obj_callback->unlocked = !(obj_callback->unlocked);
		obj_callback->tcp_client->Send(obj_callback->unlocked ? "f1" : "f0");
	}
}

static void listen_key_c(void* obj, bool down){
	if(down){
		struct_callback* obj_callback = (struct_callback*) obj;
		obj_callback->enable_streaming = !(obj_callback->enable_streaming);
		obj_callback->tcp_client->Send(obj_callback->enable_streaming ? "rs1" : "rs0");
	}
}

static void listen_key_q(void* obj, bool down){
	struct_callback* obj_callback = (struct_callback*) obj;
	obj_callback->go_on = false;
	obj_callback->tcp_client->Send("bye");
}

void init_joystick_listeners(Joystick* joystick, struct_callback* obj_callback){
	#ifdef ENABLE_SDL
		joystick->Connect_keyboard(SDLK_UP,		&listen_key_up,		obj_callback);
		joystick->Connect_keyboard(SDLK_DOWN,		&listen_key_down,	obj_callback);
		joystick->Connect_keyboard(SDLK_LEFT,		&listen_key_left,	obj_callback);
		joystick->Connect_keyboard(SDLK_RIGHT,		&listen_key_right,	obj_callback);
		joystick->Connect_keyboard(SDLK_KP_PLUS,	&listen_key_plus,	obj_callback);
		joystick->Connect_keyboard(SDLK_KP_MINUS,	&listen_key_minus,	obj_callback);
		joystick->Connect_keyboard(SDLK_c,		&listen_key_c,		obj_callback);
		joystick->Connect_keyboard(SDLK_e,		&listen_key_e,		obj_callback);
		joystick->Connect_keyboard(SDLK_i,		&listen_key_i,		obj_callback);
		joystick->Connect_keyboard(SDLK_k,		&listen_key_k,		obj_callback);
		joystick->Connect_keyboard(SDLK_l,		&listen_key_l,		obj_callback);
		joystick->Connect_keyboard(SDLK_n,		&listen_key_n,		obj_callback);
		joystick->Connect_keyboard(SDLK_q,		&listen_key_q,		obj_callback);
		joystick->Connect_keyboard(SDLK_r,		&listen_key_r,		obj_callback);
		joystick->Connect_keyboard(SDLK_s,		&listen_key_s,		obj_callback);
	#endif
}

cv::Point Scale_point(float x, float y, struct_monitor* monitor){
	float delta_min_max = monitor->max_coord - monitor->min_coord;
	int draw_x = MONITOR_BORDER / 2 + (int) (DELTA_SIZE_BORDER * (x - monitor->min_coord) / delta_min_max);
	int draw_y = MONITOR_BORDER / 2 + (int) (DELTA_SIZE_BORDER * (y - monitor->min_coord) / delta_min_max);
	return cv::Point(draw_x, draw_y);
}

void Draw_arrow(cv::Mat* img, cv::Point origin, int length, float angle, cv::Scalar color){
	cv::Point pt_arrow = origin + cv::Point(length * cos(angle), length * sin(angle));
	cv::Point pt_arrow_l = pt_arrow + cv::Point(length * cos(angle + 2.5) / 4, length * sin(angle + 2.5) / 4);
	cv::Point pt_arrow_r = pt_arrow + cv::Point(length * cos(angle - 2.5) / 4, length * sin(angle - 2.5) / 4);
	cv::line(*img, origin, pt_arrow, color, 3);
	cv::line(*img, pt_arrow, pt_arrow_l, color, 3);
	cv::line(*img, pt_arrow, pt_arrow_r, color, 3);
}

void Add_point_to_collection(vector <vector <float> > *collection, float x, float y){
	vector <float> new_point;
	new_point.push_back(x);
	new_point.push_back(y);
	collection->push_back(new_point);
}

void Resize_monitor(struct_monitor *monitor){
	if	(monitor->x < monitor->min_coord){monitor->min_coord = monitor->x;}
	else if	(monitor->x > monitor->max_coord){monitor->max_coord = monitor->x;}
	if	(monitor->y < monitor->min_coord){monitor->min_coord = monitor->y;}
	else if	(monitor->y > monitor->max_coord){monitor->max_coord = monitor->y;}
}

void Text_monitor(struct_monitor *monitor, cv::Mat *img, cv::Scalar color){
	string text_x = "x = " + to_string(monitor->x) + " m";
	string text_y = "y = " + to_string(monitor->y) + " m";
	string text_z = "z = " + to_string(monitor->z) + " m";
	string text_motor1 = "motor1 = " + to_string(monitor->motor1) + "%";
	string text_motor2 = "motor2 = " + to_string(monitor->motor2) + "%";
	string text_motor3 = "motor3 = " + to_string(monitor->motor3) + "%";
	string text_motor4 = "motor4 = " + to_string(monitor->motor4) + "%";
	string text_state = State_machine::Decode_state_str(monitor->state) + string(monitor->unlocked ? " [unlocked]" : " [LOCKED]");
	cv::putText(*img, text_state,	cv::Point(10, 20),	CV_FONT_HERSHEY_SIMPLEX, 0.5, color);
	cv::putText(*img, text_x,	cv::Point(10, 60),	CV_FONT_HERSHEY_SIMPLEX, 0.5, color);
	cv::putText(*img, text_y,	cv::Point(10, 80),	CV_FONT_HERSHEY_SIMPLEX, 0.5, color);
	cv::putText(*img, text_z,	cv::Point(10, 100),	CV_FONT_HERSHEY_SIMPLEX, 0.5, color);
	cv::putText(*img, text_motor1,	cv::Point(10, 120),	CV_FONT_HERSHEY_SIMPLEX, 0.5, color);
	cv::putText(*img, text_motor2,	cv::Point(10, 140),	CV_FONT_HERSHEY_SIMPLEX, 0.5, color);
	cv::putText(*img, text_motor3,	cv::Point(10, 160),	CV_FONT_HERSHEY_SIMPLEX, 0.5, color);
	cv::putText(*img, text_motor4,	cv::Point(10, 180),	CV_FONT_HERSHEY_SIMPLEX, 0.5, color);
}

cv::Mat Draw_monitor(struct_monitor* monitor){
	cv::Mat img_monitor = cv::Mat::zeros(500, 500, CV_8UC3);
	cv::Scalar red(0, 0, 255);
	Add_point_to_collection(&(monitor->path), monitor->x, monitor->y); 
	if(monitor->state == FOLLOW_OBJ_CAM){
		Add_point_to_collection(&(monitor->objects), monitor->x + 1. * cos(monitor->thz), monitor->y + 1. * sin(monitor->thz));
	}
	Resize_monitor(monitor);
	cv::Point pt_draw_prev;
	for(size_t i = 0; i < monitor->path.size(); i++){
		cv::Point pt_draw = Scale_point(monitor->path[i][0], monitor->path[i][1], monitor);
		if(i != 0){
			cv::line(img_monitor, pt_draw_prev, pt_draw, cv::Scalar(255, 0, 0));
		}
		pt_draw_prev = pt_draw;
		if(i == monitor->path.size() - 1){
			cv::circle(img_monitor, pt_draw, 10, red, 3);
			Draw_arrow(&img_monitor, pt_draw, 40, monitor->thz, red);
		}
	}
	for(size_t i = 0; i < monitor->objects.size(); i++){
		cv::Point pt_draw =Scale_point(monitor->objects[i][0], monitor->objects[i][1], monitor);
		cv::circle(img_monitor, pt_draw, 5, red, -1);
	}
	Text_monitor(monitor, &img_monitor, red);
	return img_monitor;
}

int main(int argc, char* argv[]){
	#if defined(ENABLE_SDL) and defined(ENABLE_TCP)

		// Manipulated objects
		Joystick joystick;
		TCP_client tcp_client_remote, tcp_client_monitor;
		struct_callback obj_callback;
		struct_monitor monitor;
		HSV_tools hsv;
		hsv.Create_HSV_params();
		const char* monitor_window = "Monitor";
		cv::namedWindow(monitor_window, cv::WINDOW_AUTOSIZE);
		cv::moveWindow(monitor_window, 0, 350);

		// Init remote controller
		hsv.tcp = &tcp_client_remote;
		hsv.Create_HSV_trackbars();
		init_joystick_listeners(&joystick, &obj_callback);
		tcp_client_remote.Configure(argv[1], 6242);
		obj_callback.tcp_client = &tcp_client_remote;
		obj_callback.go_on = true;
		obj_callback.remote_mode = true;
		obj_callback.unlocked = true;
		obj_callback.enable_streaming = false;

		// Init remote monitor
		usleep(1000000);
		tcp_client_monitor.Configure(argv[1], 6241);
		monitor.min_coord = -1.;
		monitor.max_coord = +1.;
		monitor.path.clear();

		// Job
		hsv.Send_first_HSV_params();
		while(obj_callback.go_on){
			string msg_monitor = string(tcp_client_monitor.Receive());
			size_t next;
			if(count(msg_monitor.begin(), msg_monitor.end(), '|') == 11){
				vector <string> tokens;
				for(size_t current = 0; tokens.size() < 11; current = next + 1){
					next = msg_monitor.find_first_of("|", current);
					tokens.push_back(msg_monitor.substr(current, next - current));
				}
				monitor.t		= stof(tokens[0]);
				monitor.state		= stoi(tokens[1]);
				monitor.unlocked	= (stof(tokens[2]) > 0);
				monitor.x		= stof(tokens[3]) / 1000;
				monitor.y		= stof(tokens[4]) / 1000;
				monitor.z		= stof(tokens[5]) / 1000;
				monitor.thz		= stof(tokens[6]) / 57.3;
				monitor.motor1		= (int) stof(tokens[7]);
				monitor.motor2		= (int) stof(tokens[8]);
				monitor.motor3		= (int) stof(tokens[9]);
				monitor.motor4		= (int) stof(tokens[10]);
			}
			if(monitor.t > 5){cv::imshow(monitor_window, Draw_monitor(&monitor));}
			cv::waitKey(10);
			joystick.Update_event();
		}

		// Goodbye
		tcp_client_remote.Send("bye");
		usleep(1000000);
		tcp_client_remote.Close();
		tcp_client_monitor.Close();
		hsv.Write_HSV_params();

	#else
		cout << "[Error] You need to activate SDL and TCP to use remote controller" << endl;
	#endif
}
