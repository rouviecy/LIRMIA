#include "FSM.h"

using namespace std;

FSM::FSM(){}

FSM::~FSM(){
	for(StateMap::iterator it = states.begin(); it != states.end(); ++it){
		for(int i = 0; i < it->second->transitions.size(); i++){
			delete it->second->transitions[i];
		}
		delete it->second;
	}
	for(EventMap::iterator it = events.begin(); it != events.end(); ++it){
		delete it->second;
	}
	for(GuardMap::iterator it = guards.begin(); it != guards.end(); ++it){
		delete it->second;
	}
	for(FuncMap::iterator it = actions.begin(); it != actions.end(); ++it){
		delete it->second;
	}
}

bool FSM::Add_state(string name, int id){
	if(states.count(name) > 0){
		cout << "[Warning] Trying to create state \"" + name + "\" which already exist" << endl;
		return false;
	}
	for(StateMap::iterator it = states.begin(); it != states.end(); ++it){
		if(it->second->id == id){
			cout << "[Error] Failed to create state \"" + name + "\" : id " + to_string(id) + " is already used" << endl;
		}
	}
	state* new_state = new state;
	if(new_state == NULL){
		cout << "[Error] Failed to create state \"" + name + "\" : null pointer" << endl;
		return false;
	}
	new_state->name = name;
	new_state->id = id;
	states[name] = new_state;
	return true;
}

bool FSM::Add_event(string name){
	if(events.count(name) > 0){
		cout << "[Warning] Trying to create event \"" + name + "\" which already exist" << endl;
		return false;
	}
	event* new_event = new event;
	if(new_event == NULL){
		cout << "[Error] Failed to create event \"" + name + "\" : null pointer" << endl;
		return false;
	}
	new_event->name = name;
	events[name] = new_event;
	return true;
}

bool FSM::Add_guard(string name, bool* condition){
	if(guards.count(name) > 0){
		cout << "[Warning] Trying to create guard \"" + name + "\" which already exist" << endl;
		return false;
	}
	guard* new_guard = new guard;
	if(new_guard == NULL){
		cout << "[Error] Failed to create guard \"" + name + "\" : null pointer" << endl;
		return false;
	}
	new_guard->name = name;
	new_guard->condition = condition;
	guards[name] = new_guard;
	return true;
}

bool FSM::Add_action(string name, void (*action)(void*)){
	if(actions.count(name) > 0){
		cout << "[Warning] Trying to create action \"" + name + "\" which already exist" << endl;
		return false;
	}
	func* new_action = new func;
	if(new_action == NULL){
		cout << "[Error] Failed to create action \"" + name + "\" : null pointer" << endl;
		return false;
	}
	new_action->name = name;
	new_action->action = action;
	actions[name] = new_action;
	return true;
}

bool FSM::Add_transition(string state_from, string state_to, string trigger, string condition, string action, void* obj){
	if(states.count(state_from) == 0){
		cout << "[Error] Failed to create transition from \"" + state_from + "\" to \"" + state_to + "\" : state \"" + state_from + "\" does not exist" << endl;
		return false;
	}
	if(states.count(state_to) == 0){
		cout << "[Error] Failed to create transition from \"" + state_from + "\" to \"" + state_to + "\" : state \"" + state_to + "\" does not exist" << endl;
		return false;
	}
	if(events.count(trigger) == 0){
		cout << "[Error] Failed to create transition from \"" + state_from + "\" to \"" + state_to + "\" : event \"" + trigger + "\" does not exist" << endl;
		return false;
	}
	if(!condition.empty() && guards.count(condition) == 0){
		cout << "[Error] Failed to create transition from \"" + state_from + "\" to \"" + state_to + "\" : guard \"" + condition + "\" does not exist" << endl;
		return false;
	}
	if(!action.empty() && actions.count(action) == 0){
		cout << "[Error] Failed to create transition from \"" + state_from + "\" to \"" + state_to + "\" : action \"" + action + "\" does not exist" << endl;
		return false;
	}
	transition* new_transition = new transition;
	if(new_transition == NULL){
		cout << "[Error] Failed to create transition from \"" + state_from + "\" to \"" + state_to + "\" : null pointer" << endl;
		return false;
	}
	new_transition->state_from	= state_from;
	new_transition->state_to	= state_to;
	new_transition->trigger		= trigger;
	new_transition->condition	= condition;
	new_transition->action		= action;
	new_transition->obj		= obj;
	events[trigger]->listening_transitions.push_back(new_transition);
	states[state_from]->transitions.push_back(new_transition);
	return true;
}

bool FSM::Launch(string state_init){
	if(states.count(state_init) == 0){
		cout << "[Error] Failed to launch FSM : state \"" + state_init + "\" does not exist" << endl;
		return false;
	}
	current_states.push_back(states[state_init]);
	current_states_id.push_back(states[state_init]->id);
	return true;
}

bool FSM::Call_event(string name){
	if(events.count(name) == 0){
		cout << "[Error] Event \"" + name + "\" does not exist" << endl;
		return false;
	}
	for(size_t i = 0; i < events[name]->listening_transitions.size(); i++){
		transition* processed_transition = events[name]->listening_transitions[i];
		string condition = processed_transition->condition;
		if(!condition.empty() && !(bool) *(guards[condition]->condition)){continue;}
		for(size_t j = 0; j < current_states.size(); j++){
			if(current_states[j] != states[processed_transition->state_from]){continue;}
			current_states[j] = states[processed_transition->state_to];
			current_states_id[j] = current_states[j]->id;
			string action = processed_transition->action;
			if(!action.empty() && actions[action]->action != NULL){
				actions[action]->action(processed_transition->obj);
			}
		}
	}
	return true;
}

vector <state*> FSM::Get_states(){
	vector <state*> result;
	for(StateMap::iterator it = states.begin(); it != states.end(); ++it){
		result.push_back(it->second);
	}
	return result;
}

string FSM::Get_state_name_by_id(int id){
	for(StateMap::iterator it = states.begin(); it != states.end(); ++it){
		if(it->second->id == id){
			return it->first;
		}
	}
	return "";
}

vector <state*> FSM::Get_current_states(){return current_states;}
vector <int> FSM::Get_current_states_id(){return current_states_id;}
