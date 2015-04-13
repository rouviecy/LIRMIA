/*
 * @(#)		FSM.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Provide a Finite State Machine
 * 	- First, add states, events, guards and actions you will use, with string keys
 * 	- Then, add transitions by providing :
 * 		- name of in/out states
 * 		- name of event which triggers the transition
 * 		- name of guard which block transition if false (can be empty string to never block)
 * 		- name of action which is called during transition (can be empty string for no action)
 * 		- pointer to an object (can be NULL) you want use in this function (usually, "this" self-pointer to change instance variables)
 * 	- To finish configuration, launch with the name of init state (you can call it several times)
 * 	- When FSM is working, use Call_event(string name) to change states
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>

#ifndef FSM_H
#define FSM_H

typedef struct{
	std::string state_from;
	std::string state_to;
	std::string trigger;
	std::string condition;
	std::string action;
	void* obj;	
}transition;

typedef struct{
	std::string name;
	std::vector <transition*> transitions;
}state;

typedef struct{
	std::string name;
	std::vector <transition*> listening_transitions;
}event;

typedef struct{
	std::string name;
	bool* condition;
}guard;

typedef struct{
	std::string name;
	void (*action)(void*);
}func;

typedef std::map <std::string, state*> StateMap;
typedef std::map <std::string, event*> EventMap;
typedef std::map <std::string, guard*> GuardMap;
typedef std::map <std::string, func*> FuncMap;

class FSM{

public:

	FSM();
	~FSM();

	bool Add_state(std::string name);

	bool Add_event(std::string name);

	bool Add_guard(std::string name, bool* condition);

	bool Add_action(std::string name, void (*action)(void*));

	bool Add_transition(
		std::string state_from,
		std::string state_to,
		std::string trigger,
		std::string condition,
		std::string action,
		void* obj);

	bool Launch(std::string state_init);

	bool Call_event(std::string name);

	std::vector <state*> Get_states();

	std::vector <state*> Get_current_states();

private:

	StateMap states;
	EventMap events;
	GuardMap guards;
	FuncMap actions;

	std::vector <state*> current_states;

};

#endif
