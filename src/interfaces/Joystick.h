/*
 * @(#)		Joystick.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Provide joystick input
 * 	- First, call Connect_joystick(int num_device)
 * 	- Grab pointers to data with *Get_buttons(), *Get_axes() and *Get_hats()
 * 		- buttons is an array of 0 (button up) and 1 (button down)
 * 		- axes is an array of integers from -32768 to 32767
 * 		- hats is an array of integers (SDL_HAT_... from SDL library)
 * 	- Each time you need to refresh data in these pointers, call Update_event()
 * 	- Disconnect_joystick() is called by destructor if you forget ...
 */

#ifdef ENABLE_SDL
	#include <SDL2/SDL.h>
#endif

#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#ifndef JOYSTICK
#define JOYSTICK

class Joystick{

public:

	Joystick();
	~Joystick();
	bool Connect_joystick(int num_device);
	void Disconnect_joystick();

	void Update_event();
	void Print_infos();

	int Get_nb_buttons();
	int Get_nb_axes();
	int Get_nb_hats();
	int *Get_buttons();
	int *Get_axes();
	int *Get_hats();

private:

	#ifdef ENABLE_SDL
		SDL_Event event;
		SDL_Joystick *joystick;
	#endif
	int num_device;
	int nb_buttons, nb_axes, nb_hats;
	int *buttons, *axes, *hats;

};

#endif
