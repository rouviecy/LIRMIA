/*
 * @(#)		Maestro.h
 * @version	2.0
 * @autor	C. Rouvière
 */

/**
 * Master class to manage all parts of a robot
 */

#ifndef MAESTRO
#define MAESTRO

#include "ComDraw.h"
#include "ComThread.h"

class Maestro{

public:

	Maestro();

protected:

	void Add_thread(ComThread* thread, std::string name, int dt_microseconds);
	void Link_all();
	void Launch_all();
	void Join_all();
	void Draw(std::string filename);

private:

	Share share;
	ComDraw drawer;
	std::vector <ComThread*> threads;

};

#endif
