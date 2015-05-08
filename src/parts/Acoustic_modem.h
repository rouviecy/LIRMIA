/*
 * @(#)		Acoustic_modem.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Communicate to other robots with acoustic modem with serial
 */

#ifndef ACOUSTIC_MODEM
#define ACOUSTIC_MODEM

#include "../core/ComThread.h"

class Acoustic_modem : public ComThread{

public:

	Acoustic_modem();
	~Acoustic_modem();

private:

	void On_start();
	void Job();
	void IO();
	
	float xyz[3];
	float thxyz[3];

};

#endif
