/*
 * @(#)		Echosonder.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Echosonder measures distance/velocity to obstacles with serial
 */

#ifndef ECHOSONDER
#define ECHOSONDER

#include "../core/ComThread.h"

class Echosonder : public ComThread{

public:

	Echosonder();
	~Echosonder();

private:

	void On_start();
	void Job();
	void IO();

	float obst_x,  obst_y;
	float obst_vx, obst_vy;

};

#endif
