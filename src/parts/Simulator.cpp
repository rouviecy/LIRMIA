#include "Simulator.h"

using namespace std;

Simulator::Simulator() : ComThread(){
	last_t = -1.;
	for(int i = 0; i < 3; i++){simu_xyz[i] = 0.; simu_thxyz[i] = 0.;}
	for(int i = 0; i < 6; i++){acceleration[i] = 0.; velocity[i] = 0.;}
}

Simulator::~Simulator(){}

void Simulator::On_start(){}

void Simulator::IO(){
	Link_input("t",			COMFLOAT,	1, &t);
	Link_input("motor",		COMFLOAT,	4, motor);

	Link_output("simu_xyz",		COMFLOAT,	3, simu_xyz);
	Link_output("simu_thxyz",	COMFLOAT,	3, simu_thxyz);
}

void Simulator::Job(){
	#ifdef ENABLE_SIMULATOR
		Critical_receive();

		acceleration[0] += (motor[0] + motor[1]) * 0.1;	// X-translation
		acceleration[2] += (motor[2] + motor[3]) * 0.1;	// Z-translation
		acceleration[4] += (motor[2] - motor[3]) * 1.;	// Y-rotation
		acceleration[5] += (motor[0] - motor[1]) * 1.;	// Z-rotation

		for(int i = 0; i < 6; i++){
			acceleration[i] = acceleration[i] * 0.95;
			velocity[i] = velocity[i] * 0.995;
			if(i < 3)	{Saturate(&(acceleration[i]), 5., false);	Saturate(&(velocity[i]), 1., false);}
			else		{Saturate(&(acceleration[i]), 50., false);	Saturate(&(velocity[i]), 180., false);}
		}

		if(last_t > 0){
			float dt = t - last_t;
			velocity[3] += 10. * acceleration[3] * dt;	simu_thxyz[0] += velocity[3] * dt;
			velocity[4] += 10. * acceleration[4] * dt;	simu_thxyz[1] += velocity[4] * dt;
			velocity[5] += 10. * acceleration[5] * dt;	simu_thxyz[2] += velocity[5] * dt;
			velocity[0] += acceleration[0] * dt;		simu_xyz[0] += cos(0.01745 * simu_thxyz[2]) * velocity[0] * dt;
			velocity[1] += acceleration[1] * dt;		simu_xyz[1] += sin(0.01745 * simu_thxyz[2]) * velocity[0] * dt;
			velocity[2] += acceleration[2] * dt;		simu_xyz[2] += velocity[2] * dt;
			Saturate(&(simu_thxyz[0]), 90., true);
			Saturate(&(simu_thxyz[1]), 180., true);
			Saturate(&(simu_thxyz[2]), 180., true);
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
