#include "Joystick.h"

using namespace std;

Joystick::Joystick(){
	#ifdef ENABLE_SDL
		joystick = NULL;
	#endif
	num_device = -1;
	buttons = NULL;
	axes = NULL;
	hats = NULL;
}

Joystick::~Joystick(){
	Disconnect_joystick();
	#ifdef ENABLE_SDL
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
		buttons	= (int*)	malloc(sizeof(int) * nb_buttons);
		axes	= (int*)	malloc(sizeof(int) * nb_axes);
		hats	= (int*)	malloc(sizeof(int) * nb_hats);
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

void Joystick::Update_event(){
	#ifdef ENABLE_SDL
		while(SDL_PollEvent(&event)){
			if(joystick == NULL || (event.jbutton.which != num_device && event.jaxis.which != num_device && event.jhat.which != num_device)){continue;}
			switch(event.type){
				case SDL_JOYBUTTONDOWN:	buttons[event.jbutton.button] = 1;			break;
				case SDL_JOYBUTTONUP:	buttons[event.jbutton.button] = 0;			break;
				case SDL_JOYAXISMOTION:	axes[event.jaxis.axis] = event.jaxis.value;	break;
				case SDL_JOYHATMOTION:	hats[event.jhat.hat] = event.jhat.value;	break;
				default:															break;
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

int Joystick::Get_nb_buttons(){return nb_buttons;}
int Joystick::Get_nb_axes(){return nb_axes;}
int Joystick::Get_nb_hats(){return nb_hats;}
int *(Joystick::Get_buttons()){return buttons;}
int *(Joystick::Get_axes()){return axes;}
int *(Joystick::Get_hats()){return hats;}
