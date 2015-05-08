#include "ComThread.h"

using namespace std;

ComThread::ComThread(){
	dt_microseconds = 1000000;
}

void ComThread::Link(Share *s){
	this->s = s;
	IO();
}

void ComThread::Launch(){
	job_go_on = true;
	thr = thread(&ComThread::Loop_job, this);
}

void ComThread::Join(){
	job_go_on = false;
	thr.join();
}

void ComThread::Loop_job(){
	On_start();
	if(dt_microseconds > 0){
		while(job_go_on){
			Job();
			usleep(dt_microseconds);
		}
	}
	else{
		Job();
	}
}

void ComThread::Link_input(string key, T_DATA data_type, int size, void *p_data){
	if(size <= 0){cout << "[Error] Trying to create data \"" + key + "\" with wrong size " + to_string(size) + " < 1" << endl; return;}
	if(s->Create_data(key, data_type, size)){critical_input[key] = p_data;}
	input_types[key] = Share::Convert_type_str(data_type, size);
}

void ComThread::Link_output(string key, T_DATA data_type, int size, void *p_data){
	if(size <= 0){cout << "[Error] Trying to create data \"" + key + "\" with wrong size " + to_string(size) + " < 1" << endl; return;}
	if(s->Create_data(key, data_type, size)){critical_output[key] = p_data;}
	output_types[key] = Share::Convert_type_str(data_type, size);
}

void ComThread::Critical_receive(){s->Update(critical_input, true);}
void ComThread::Critical_send(){s->Update(critical_output, false);}

void ComThread::Set_freq(int dt_microseconds){this->dt_microseconds = dt_microseconds;}
void ComThread::Set_name(string name){this->name = name;}
string ComThread::Get_name(){return name;}
float ComThread::Get_freq(){return dt_microseconds > 0 ? 1000000. / (float) dt_microseconds : 0.;}

map <string, string> ComThread::Get_inputs(){return input_types;}
map <string, string> ComThread::Get_outputs(){return output_types;}
