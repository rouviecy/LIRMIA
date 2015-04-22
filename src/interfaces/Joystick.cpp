#include "Joystick.h"

using namespace std;

Joystick::Joystick(){
	#ifdef ENABLE_SDL
		joystick = NULL;
		SDL_Init(SDL_INIT_VIDEO);
		window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
		renderer = SDL_CreateRenderer(window, -1, 0);
	#endif
	num_device = -1;
	buttons = NULL;
	axes = NULL;
	hats = NULL;
}

Joystick::~Joystick(){
	Disconnect_joystick();
	#ifdef ENABLE_SDL
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	#endif
}

bool Joystick::Connect_joystick(int num_device){
	#ifdef ENABLE_SDL
		if(SDL_Init(SDL_INIT_JOYSTICK) < 0){return false;}
		if(SDL_NumJoysticks() < num_device - 1){return false;}
		joystick = SDL_JoystickOpen(num_device);
		this->num_device = num_device;
		nb_buttons	= SDL_JoystickNumButtons(joystick);
		nb_axes		= SDL_JoystickNumAxes(joystick);
		nb_hats		= SDL_JoystickNumHats(joystick);
		buttons	= (int*) malloc(sizeof(int) * nb_buttons);
		axes	= (int*) malloc(sizeof(int) * nb_axes);
		hats	= (int*) malloc(sizeof(int) * nb_hats);
		for(int i = 0; i < nb_buttons; i++)	{buttons[i] = 0;}
		for(int i = 0; i < nb_axes; i++)	{axes[i] = 0;}
		for(int i = 0; i < nb_hats; i++)	{hats[i] = SDL_HAT_CENTERED;}
		SDL_JoystickEventState(SDL_ENABLE);
		return true;
	#else
		cout << "[Warning] You are trying to use joystick control without SDL enabled." << endl;
		return false;
	#endif
}

bool Joystick::Connect_keyboard(int key, void (*callback) (void*, bool), void* obj){
	#ifdef ENABLE_SDL
		if(keyboard_subscribes.count(key) > 0){
			cout << "[Warning] SDL keyboard subscribe for key " << key << " already exists : abort mission !" << endl;
			return false;
		}
		subscribe* new_subscribe = new subscribe;
		new_subscribe->callback = callback;
		new_subscribe->obj = obj;
		new_subscribe->is_down = false;
		keyboard_subscribes[key] = new_subscribe;
		return true;
	#else
		cout << "[Warning] You are trying to use keyboard control without SDL enabled." << endl;
	#endif
}

void Joystick::Disconnect_joystick(){
	if(num_device < 0){return;}
	num_device = -1;
	free(buttons);
	free(axes);
	free(hats);
	#ifdef ENABLE_SDL
		SDL_JoystickClose(joystick);
	#endif
}

void Joystick::Disconnect_keyboard(){
	#ifdef ENABLE_SDL
		for(std::map <SDL_Keycode, subscribe*> ::iterator it = keyboard_subscribes.begin(); it != keyboard_subscribes.end(); ++it){
			delete it->second;
		}
	#else
		cout << "[Warning] You are trying to use keyboard control without SDL enabled." << endl;
	#endif
}

void Joystick::Update_event(){
	#ifdef ENABLE_SDL
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_KEYDOWN){Check_keyboard_subscribe(event.key.keysym.sym, true);}
			if(event.type == SDL_KEYUP){Check_keyboard_subscribe(event.key.keysym.sym, false);}
			if(joystick == NULL || (event.jbutton.which != num_device && event.jaxis.which != num_device && event.jhat.which != num_device)){continue;}
			switch(event.type){
				case SDL_JOYBUTTONDOWN:	buttons[event.jbutton.button] = 1;		break;
				case SDL_JOYBUTTONUP:	buttons[event.jbutton.button] = 0;		break;
				case SDL_JOYAXISMOTION:	axes[event.jaxis.axis] = event.jaxis.value;	break;
				case SDL_JOYHATMOTION:	hats[event.jhat.hat] = event.jhat.value;	break;
				default:								break;
			}
		}
	#endif
}

void Joystick::Print_infos(){
	#ifdef ENABLE_SDL
		if(num_device < 0){
			cout << "Joystick disconnected" << endl;
		}
		else{
			cout << "Joystick connected : " << SDL_JoystickName(joystick) << endl;
			cout << "\tNb buttons : " << nb_buttons << endl;
			cout << "\tNb axes : " << nb_axes << endl;
			cout << "\tNb hats : " << nb_hats << endl;
		}
	#else
		cout << "SDL disabled" << endl;
	#endif
}

#ifdef ENABLE_SDL
void Joystick::Check_keyboard_subscribe(SDL_Keycode key, bool downing){
	if(keyboard_subscribes.count(key) > 0){
		if(downing && !(keyboard_subscribes[key]->is_down)){
			keyboard_subscribes[key]->callback(keyboard_subscribes[key]->obj, true);
			keyboard_subscribes[key]->is_down = true;
		}
		if(!downing && keyboard_subscribes[key]->is_down){
			keyboard_subscribes[key]->callback(keyboard_subscribes[key]->obj, false);
			keyboard_subscribes[key]->is_down = false;
		}
	}
}
#endif

int Joystick::Get_nb_buttons(){return nb_buttons;}
int Joystick::Get_nb_axes(){return nb_axes;}
int Joystick::Get_nb_hats(){return nb_hats;}
int *(Joystick::Get_buttons()){return buttons;}
int *(Joystick::Get_axes()){return axes;}
int *(Joystick::Get_hats()){return hats;}
