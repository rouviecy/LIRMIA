/*
 * @(#)		Remote.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Remote control with TCP
 */

#ifndef REMOTE
#define REMOTE

#include "../core/ComThread.h"

class Remote : public ComThread{

public:

	Remote();
	~Remote();

private:

	void On_start();
	void Job();
	void IO();
	
	float remote;

};

#endif
