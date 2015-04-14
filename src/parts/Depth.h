/*
 * @(#)		Depth.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Direct depth measurement with I2C
 */

#ifndef DEPTH
#define DEPTH

#include "../core/ComThread.h"

class Depth : public ComThread{

public:

	Depth();
	~Depth();

private:

	void On_start();
	void Job();
	void IO();
	
	float depth;

};

#endif
