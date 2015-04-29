#include "Imu.h"

using namespace std;

Imu::Imu() : ComThread(){
	#if defined(ENABLE_IMU) and not defined(ENABLE_SERIAL_ARDUINO)
		cout << "[Warning] You are trying to use IMU without serial enabled : IMU will be disabled" << endl;
	#endif
	imu_thx = 0.;  imu_thy = 0.;  imu_thz = 0.;
	msg.clear();
	header_size = 0;
}

Imu::~Imu(){}

void Imu::On_start(){}

void Imu::IO(){
	Link_output("imu_thx", &imu_thx);
	Link_output("imu_thy", &imu_thy);
	Link_output("imu_thz", &imu_thz);
}

void Imu::Job(){
	#if defined(ENABLE_IMU) && defined(ENABLE_SERIAL_ARDUINO)
		serial->Lock();
		char* answer = serial->Serial_read();
		for(int i = 0; i < SERIAL_BUFFER_LEN; i++){
			switch(header_size){
				case 0:	header_size = (answer[i] == '#') ? header_size + 1 : 0; break;
				case 1:	header_size = (answer[i] == 'Y') ? header_size + 1 : 0; break;
				case 2:	header_size = (answer[i] == 'P') ? header_size + 1 : 0; break;
				case 3:	header_size = (answer[i] == 'R') ? header_size + 1 : 0; break;
				case 4:	header_size = (answer[i] == '=') ? header_size + 1 : 0; break;
				default:
				if(answer[i] == '\n' && header_size > 4){
					msg.push_back(',');
					msg.push_back('\0');
					size_t next;
					string msg_ypr = string(msg.data());
					if(count(msg_ypr.begin(), msg_ypr.end(), ',') == 3){
						vector <string> tokens;
						for(size_t current = 0; tokens.size() < 3; current = next + 1){
							next = msg_ypr.find_first_of(",", current);
							tokens.push_back(msg_ypr.substr(current, next - current));
						}
						if(tokens[0].length() > 2 && tokens[1].length() > 2 && tokens[2].length() > 2){
							imu_thz = stof(tokens[0]);
							imu_thx = stof(tokens[1]);
							imu_thy = stof(tokens[2]);
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

void Imu::Set_serial(Serial* serial){this->serial = serial;}
