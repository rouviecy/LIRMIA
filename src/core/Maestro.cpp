#include "Maestro.h"

using namespace std;

Maestro::Maestro(){}

void Maestro::Add_thread(ComThread* thread, string name, int dt_microseconds){
	thread->Set_name(name);
	thread->Set_freq(dt_microseconds);
	threads.push_back(thread);
}

void Maestro::Link_all(){
	for(size_t i = 0; i < threads.size(); i++){
		threads[i]->Link(&share);
	}
}

void Maestro::Launch_all(){
	for(size_t i = 0; i < threads.size(); i++){
		threads[i]->Launch();
	}
}

void Maestro::Join_all(){
	for(size_t i = 0; i < threads.size(); i++){
		threads[i]->Join();
	}
}

void Maestro::Draw(string filename){
	drawer.Draw_threads(filename, threads);
}
