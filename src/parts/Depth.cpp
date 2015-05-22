#include "Depth.h"

using namespace std;

Depth::Depth() : ComThread(){
	depth = 0.;
	#if defined(ENABLE_DEPTH) and not defined(ENABLE_SERIAL_ARDUINO)
		cout << "[Warning] You are trying to use depth sensor without Arduino serial enabled : depth sensor will be disabled" << endl;
	#endif
	msg.clear();
	header_size = 0;
}

Depth::~Depth(){}

void Depth::On_start(){}

void Depth::IO(){
	Link_input("t",		COMFLOAT, 1, &t);
	Link_output("depth",	COMFLOAT, 1, &depth);
}

void Depth::Job(){
	Critical_receive();
	#if defined(ENABLE_DEPTH) and defined(ENABLE_SERIAL_ARDUINO)
		serial->Lock();
		char* answer = serial->Serial_read();
		for(int i = 0; i < SERIAL_BUFFER_LEN; i++){
			switch(header_size){
				case 0:	header_size = (answer[i] == '#') ? header_size + 1 : 0; break;
				case 1:	header_size = (answer[i] == 'D') ? header_size + 1 : 0; break;
				case 2:	header_size = (answer[i] == 'E') ? header_size + 1 : 0; break;
				case 3:	header_size = (answer[i] == 'P') ? header_size + 1 : 0; break;
				case 4:	header_size = (answer[i] == '=') ? header_size + 1 : 0; break;
				default:
				if(answer[i] == '\n' && header_size > 4){
					msg.push_back(',');
					msg.push_back('\0');
					size_t next;
					string msg_ypr = string(msg.data());
					if(count(msg_ypr.begin(), msg_ypr.end(), ',') == 1){
						vector <string> tokens;
						for(size_t current = 0; tokens.size() < 1; current = next + 1){
							next = msg_ypr.find_first_of(",", current);
							tokens.push_back(msg_ypr.substr(current, next - current));
						}
						if(tokens[0].length() > 3){
							depth = stof(tokens[i]);
							Critical_send();
						}
					}
					header_size = 0;
					msg.clear();
					continue;
				}
				msg.push_back(answer[i]);
				break;
			}
		}
		serial->Unlock();
	#endif
}


void Depth::Set_serial(Serial* serial){this->serial = serial;}
