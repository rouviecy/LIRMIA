#include "State.h"

using namespace std;

State::State() : ComThread(){
	xyz[0] = 0.;
	xyz[1] = 0.;
	xyz[2] = 0.;
	thxyz[0] = 0.;
	thxyz[1] = 0.;
	thxyz[2] = 0.;
	vthxyz[0] = 0.;
	vthxyz[1] = 0.;
	vthxyz[2] = 0.;
	
	xk_1 = 0.;
	vk_1 = 0.;
}

State::~State(){}

void State::On_start(){
	old_t = -1;
}

void State::IO(){
	Link_input("compass_thxyz",	COMFLOAT, 3, compass_thxyz);
	Link_input("depth",		COMFLOAT, 1, &depth);
	Link_input("t",			COMFLOAT, 1, &t);

	Link_output("xyz",		COMFLOAT, 3, xyz);
	Link_output("thxyz",		COMFLOAT, 3, thxyz);
	Link_output("vthxyz",		COMFLOAT, 3, vthxyz);
	
}

void State::Job(){
	Critical_receive();
	thxyz[0] = compass_thxyz[0];
	thxyz[1] = compass_thxyz[1];
	thxyz[2] = compass_thxyz[2];
	//-------------------  Filtro del angulo ------------------------------------------
		float xm = thxyz[2];
                float a=0.2;
                float b=0.3;
                float xk, xk_1, vk, rk, h;
                float vk_1=0;
                float dt=0.125;              
                
                xk_1=200;
		//float dt = t - old_t;
		xk = xk_1 + ( vk_1 * dt );
		vk = vk_1;

		rk = xm - xk;

		xk =xk + a*rk;
		vk =vk + (b*rk)/dt;

		xk_1 = xk;
		vk_1 = vk;
		
	
	//old_t = t;
	vthxyz[2] = vk_1;
	xyz[2] = depth;
	Critical_send();
cout << dt << endl;

}
