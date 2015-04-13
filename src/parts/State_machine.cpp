#include "State_machine.h"

using namespace std;

State_machine::State_machine() : ComThread(){

	my_guard = true;

	fsm.Add_state("remote");
	fsm.Add_state("stay");
	fsm.Add_state("down");
	fsm.Add_state("explore");
	fsm.Add_state("follow");
	fsm.Add_state("up");

	fsm.Add_event("go_to_autonomous");
	fsm.Add_event("go_down");
	fsm.Add_event("begin_explore");
	fsm.Add_event("found_something");
	fsm.Add_event("stop_follow");
	fsm.Add_event("go_up");
	fsm.Add_event("reach_surface");
	fsm.Add_event("go_to_remote");

	fsm.Add_guard("guard", &my_guard);

				// state_from	// state_to	// trigger		// guard	// action	// object
	fsm.Add_transition(	"remote",	"stay",		"go_to_autonomous",	"guard",	"",		(void*) this);
	fsm.Add_transition(	"stay",		"down",		"go_down",		"guard",	"",		(void*) this);
	fsm.Add_transition(	"down",		"explore",	"begin_explore",	"guard",	"",		(void*) this);
	fsm.Add_transition(	"explore",	"follow",	"found_something",	"guard",	"",		(void*) this);
	fsm.Add_transition(	"follow",	"explore",	"stop_follow",		"guard",	"",		(void*) this);
	fsm.Add_transition(	"explore",	"up",		"go_up",		"guard",	"",		(void*) this);
	fsm.Add_transition(	"up",		"stay",		"reach_surface",	"guard",	"",		(void*) this);
	fsm.Add_transition(	"stay",		"remote",	"go_to_remote",		"guard",	"",		(void*) this);

	fsm.Launch("remote");
	drawer.Draw_FSM( "FSM", &fsm);

}

State_machine::~State_machine(){}

void State_machine::On_start(){}

void State_machine::IO(){
	Link_input("my_data", &data_in);
}

void State_machine::Job(){
	Critical_receive();
	cout << "Received data : " << data_in << endl;
}
