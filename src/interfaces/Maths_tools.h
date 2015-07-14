/*
 * @(#)		Maths_tools.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Some usefull tools for Automatics ...
 */

#ifndef MATHS_TOOLS
#define MATHS_TOOLS

#include <math.h>

#define saturation(minimum, value, maximum) (value > maximum ? maximum : (value < minimum ? minimum : value))

inline float diff_angles_deg(float a, float b){
	float result = fmod(b - a + 180, 360);
	if(result < 0){result += 360;}
	return result - 180;
}

#endif
