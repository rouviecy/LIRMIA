/*
 * @(#)		FSMDraw.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Draw Finite State Machine
 */

#ifndef FSMDRAW
#define FSMDRAW

#include <iostream>
#include <fstream>
#include "FSM.h"

#ifdef ENABLE_GRAPHVIZ
	#include <gvc.h>
#endif

class FSMDraw{

public:

	FSMDraw();
	void Draw_FSM(std::string name, FSM* fsm);

private:

	void Generate_dot(std::string name, FSM* fsm);
	void Generate_svg(std::string name);

};

#endif
