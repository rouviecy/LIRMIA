#include "Share.h"

using namespace std;

Share::Share(){}

Share::~Share(){
	for(PDataMap::iterator it = data.begin(); it != data.end(); ++it){
		switch(it->second.data_type){
			case COMBOOL:	delete[] (bool*)	(it->second.p_data);	break;
			case COMINT:	delete[] (int*)		(it->second.p_data);	break;
			case COMFLOAT:	delete[] (float*)	(it->second.p_data);	break;
			default:							break;
		}
	}
}

void Share::Lock(){mu.lock();}
void Share::Unlock(){mu.unlock();}

bool Share::Create_data(string key, T_DATA data_type, int size){
	Lock();
	if(data.count(key) == 0){
		DATA new_data;
		new_data.data_type = data_type;
		new_data.size = size;
		switch(data_type){
			case COMBOOL:	new_data.p_data = (void*) (new bool[size]);	break;
			case COMINT:	new_data.p_data = (void*) (new int[size]);	break;
			case COMFLOAT:	new_data.p_data = (void*) (new float[size]);	break;
			default:	new_data.p_data = NULL;				return false;
		}
		data[key] = new_data;
	}
	else{
		if(data[key].data_type != data_type){
			cout << "[Error] Wrong link for data \"" + key + "\" : data types are not the same" << endl;
			Unlock();
			return false;
		}
		if(data[key].size != size){
			cout << "[Error] Wrong link for data \"" + key + "\" : sizes are not the same" << endl;
			Unlock();
			return false;
		}
	}
	Unlock();
	return true;
}

void Share::Update(PVoidMap io_data, bool is_input){
	Lock();
	for(PVoidMap::iterator it = io_data.begin(); it != io_data.end(); ++it){
		void* a = it->second;
		void* b = data[it->first].p_data;
		for(int i = 0; i < (data[it->first]).size; i++){
			switch(data[it->first].data_type){
				case COMBOOL:	if(is_input)	{((bool*)a)[i]	= ((bool*)b)[i];}	else{((bool*)b)[i]	= ((bool*)a)[i];}	break;
				case COMINT:	if(is_input)	{((int*)a)[i]	= ((int*)b)[i];}	else{((int*)b)[i]	= ((int*)a)[i];}	break;
				case COMFLOAT:	if(is_input)	{((float*)a)[i]	= ((float*)b)[i];}	else{((float*)b)[i]	= ((float*)a)[i];}	break;
				default:														break;
			}
		}
	}
	Unlock();
}
