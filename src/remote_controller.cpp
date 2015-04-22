#include "interfaces/TCP_client.h"
#include "interfaces/Joystick.h"

using namespace std;

typedef struct{
	TCP_client* tcp_client;
	bool go_on;
	bool remote_mode;
} struct_callback;

static void listen_key_up(void* obj, bool down){
	struct_callback* obj_callback = (struct_callback*) obj;
	obj_callback->tcp_client->Send("ra" + to_string(down));
}

static void listen_key_down(void* obj, bool down){
	struct_callback* obj_callback = (struct_callback*) obj;
	obj_callback->tcp_client->Send("rb" + to_string(down));
}

static void listen_key_left(void* obj, bool down){
	struct_callback* obj_callback = (struct_callback*) obj;
	obj_callback->tcp_client->Send("rl" + to_string(down));
}

static void listen_key_right(void* obj, bool down){
	struct_callback* obj_callback = (struct_callback*) obj;
	obj_callback->tcp_client->Send("rr" + to_string(down));
}

static void listen_key_plus(void* obj, bool down){
	struct_callback* obj_callback = (struct_callback*) obj;
	obj_callback->tcp_client->Send("ru" + to_string(down));
}

static void listen_key_minus(void* obj, bool down){
	struct_callback* obj_callback = (struct_callback*) obj;
	obj_callback->tcp_client->Send("rd" + to_string(down));
}

static void listen_key_a(void* obj, bool down){
	if(down){
		struct_callback* obj_callback = (struct_callback*) obj;
		obj_callback->remote_mode = !(obj_callback->remote_mode);
		obj_callback->tcp_client->Send(obj_callback->remote_mode ? "r1" : "r0");
	}
}

static void listen_key_q(void* obj, bool down){
	struct_callback* obj_callback = (struct_callback*) obj;
	obj_callback->go_on = false;
	obj_callback->tcp_client->Send("bye");
}

int main(){
	#ifdef ENABLE_SDL
		Joystick joystick;
		TCP_client tcp_client;
		struct_callback obj_callback;
		joystick.Connect_keyboard(SDLK_UP,		&listen_key_up,		&obj_callback);
		joystick.Connect_keyboard(SDLK_DOWN,		&listen_key_down,	&obj_callback);
		joystick.Connect_keyboard(SDLK_LEFT,		&listen_key_left,	&obj_callback);
		joystick.Connect_keyboard(SDLK_RIGHT,		&listen_key_right,	&obj_callback);
		joystick.Connect_keyboard(SDLK_KP_PLUS,		&listen_key_plus,	&obj_callback);
		joystick.Connect_keyboard(SDLK_KP_MINUS,	&listen_key_minus,	&obj_callback);
		joystick.Connect_keyboard(SDLK_a,		&listen_key_a,		&obj_callback);
		joystick.Connect_keyboard(SDLK_q,		&listen_key_q,		&obj_callback);
		tcp_client.Configure("127.0.0.1", 4242);
		obj_callback.tcp_client = &tcp_client;
		obj_callback.go_on = true;
		obj_callback.remote_mode = true;
		while(obj_callback.go_on){
			usleep(10000);
			joystick.Update_event();
		}
		tcp_client.Send("bye");
		usleep(1000000);
		tcp_client.Close();
	#else
		cout << "[Error] You need to activate SDL to use remote" << endl;
	#endif
}
