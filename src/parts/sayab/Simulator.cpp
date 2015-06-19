#include "Simulator.h"

using namespace std;

Simulator::Simulator() : ComThread(){
	last_t = -1.;
	simu_xy[0] = 0.;
	simu_xy[1] = 0.;
	simu_thz = 0.;
	for(int i = 0; i < 3; i++){acceleration[i] = 0.; velocity[i] = 0.;}
}

Simulator::~Simulator(){}

void Simulator::On_start(){}

void Simulator::IO(){
	Link_input("t",			COMFLOAT,	1, &t);
	Link_input("motor",		COMFLOAT,	1, &motor);
	Link_input("rudder",		COMFLOAT,	1, &rudder);
	Link_input("bow_thruster",	COMFLOAT,	1, &bow_thruster);

	Link_output("simu_xy",		COMFLOAT,	2, simu_xy);
	Link_output("simu_thz",		COMFLOAT,	1, &simu_thz);
}

void Simulator::Job(){
	#ifdef ENABLE_SIMULATOR
		Critical_receive();

		acceleration[0] += motor * 0.05;				// X-translation
		acceleration[1] = 0.;						// Y-translation
		acceleration[2] += rudder * motor * 5. + bow_thruster * 5.;	// Z-rotation

		for(int i = 0; i < 3; i++){
			acceleration[i] = acceleration[i] * 0.95;
			velocity[i] = velocity[i] * 0.995;
			if(i < 2)	{Saturate(&(acceleration[i]), 3., false);	Saturate(&(velocity[i]), 1., false);}
			else		{Saturate(&(acceleration[i]), 10., false);	Saturate(&(velocity[i]), 180., false);}
		}

		if(last_t > 0){
			float dt = t - last_t;
			velocity[2] += 10. * acceleration[2] * dt;	simu_thz += velocity[2] * dt;
			velocity[0] += acceleration[0] * dt;
			velocity[1] += acceleration[1] * dt;
			simu_xy[0] += (cos(0.01745 * simu_thz) * velocity[0] - sin(0.01745 * simu_thz) * velocity[1]) * dt;
			simu_xy[1] += (sin(0.01745 * simu_thz) * velocity[0] + cos(0.01745 * simu_thz) * velocity[1]) * dt;
			Saturate(&(simu_thz), 180., true);
		}

		last_t = t;

		Critical_send();
	#endif
}

void Simulator::Saturate(float* number, float maximum, bool cycle){
	if(cycle){
		while(*number > +maximum){*number -= 2 * maximum;}
		while(*number < -maximum){*number += 2 * maximum;}
	}
	else{
		if(fabs(*number) > maximum){*number = *number > 0 ? +maximum : -maximum;}
	}
}
