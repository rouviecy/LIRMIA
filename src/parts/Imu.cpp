#include "Imu.h"

using namespace std;

Imu::Imu() : ComThread(){
	imu_thx = 0.;  imu_thy = 0.;  imu_thz = 0.;
	imu_vthx = 0.; imu_vthy = 0.; imu_vthz = 0.;
}

Imu::~Imu(){}

void Imu::On_start(){
	unsigned char imu_request[8];
	imu_request[0] = 0x62; // serial
	imu_request[1] = 0x73; // 'S'
	imu_request[2] = 0x6E; // 'N'
	imu_request[3] = 0x70; // 'P'
	imu_request[4] = 0x00; // packet type
	imu_request[5] = 0xB1; // factory settings
	imu_request[6] = 0x02; // checksum high byte
	imu_request[7] = 0x02; // checksum low byte
	serial->Serial_write(imu_request, 8);
	imu_request[0] = 0x62; // serial
	imu_request[1] = 0x73; // 'S'
	imu_request[2] = 0x6E; // 'N'
	imu_request[3] = 0x70; // 'P'
	imu_request[4] = 0x00; // packet type
	imu_request[5] = 0xAB; // write config to flash
	imu_request[6] = 0x01; // checksum high byte
	imu_request[7] = 0xFC; // checksum low byte
	serial->Serial_write(imu_request, 8);
	imu_request[0] = 0x62; // serial
	imu_request[1] = 0x73; // 'S'
	imu_request[2] = 0x6E; // 'N'
	imu_request[3] = 0x70; // 'P'
	imu_request[4] = 0x00; // packet type
	imu_request[5] = 0xB1; // factory settings
	imu_request[6] = 0x02; // checksum high byte
	imu_request[7] = 0x02; // checksum low byte
	serial->Serial_write(imu_request, 8);
}

void Imu::IO(){
	Link_output("imu_thx", &imu_thx);   Link_output("imu_thy", &imu_thy);   Link_output("imu_thz", &imu_thz);
	Link_output("imu_vthx", &imu_vthx); Link_output("imu_vthy", &imu_vthy); Link_output("imu_vthz", &imu_vthz);
}

void Imu::Job(){
	#if defined ENABLE_SERIAL && defined ENABLE_IMU
		while(true){
			usleep(1000);
			char* answer = serial->Serial_read();
			for(int i = 0; i < SERIAL_BUFFER_LEN; i++){
				if(msg.size() == 0){
					if(answer[i] == 115)	{msg.push_back(answer[i]);}
					continue;
				}
				if(msg.size() == 1){
					if(answer[i] == 110)	{msg.push_back(answer[i]);}
					else			{msg.clear();}
					continue;
				}
				if(msg.size() == 2){
					if(answer[i] == 112)	{msg.push_back(answer[i]);}
					else			{msg.clear();}
					continue;
				}
				msg.push_back(answer[i]);
				if(msg.size() == 15){
					for(size_t j = 0; j < 15; j++){
						cout << msg[j] << " | ";
					}
					cout << endl;

					short yaw1		= (msg[8] << 8) | msg[9];
					short pitch1		= (msg[10] << 8) | msg[11];
					short roll1		= (msg[12] << 8) | msg[13];

					float YAW = (float) yaw1 * FAC_ANG_IMU;
					float PITCH = (float) pitch1 * FAC_ANG_IMU;    
					float ROLL = (float) roll1 * FAC_ANG_IMU;

					cout << (int) YAW << "\t" << (int) PITCH << "\t" << (int) ROLL << endl;

					msg.clear();
				}
			}

		}

	#endif
}

void Imu::Generate_YPR(void* obj, char* answer){
	Imu* imu = (Imu*) obj;
cout << string(answer).length() << endl;
	short yaw1		= (answer[11] << 8) | answer[12];
	short pitch1		= (answer[13] << 8) | answer[14];
	short roll1		= (answer[15] << 8) | answer[16];

	short yawrate1		= (answer[17] << 8) | answer[18];
	short pitchrate1	= (answer[19] << 8) | answer[20];
	short rollrate1		= (answer[21] << 8) | answer[22];
	short accz1		= (answer[23] << 8) | answer[24];

	float YAW = (float) yaw1 * FAC_ANG_IMU;
	float PITCH = (float) pitch1 * FAC_ANG_IMU;    
	float ROLL = (float) roll1 * FAC_ANG_IMU;

	float YAWRATE = yawrate1 * FAC_RAT_IMU;
	float pitchrate = pitchrate1 * FAC_RAT_IMU;    
	float rollrate = rollrate1 * FAC_RAT_IMU;
	float accz = accz1 * FAC_ACC_IMU;
cout << YAW << " | " << PITCH << " | " << ROLL << endl;
	// thx = ???
	// thy = ???
	// thz = ???
	// vthx = ???
	// vthy = ???
	// vthz = ???
	imu->Critical_send();	
}

void Imu::Set_serial(Serial* serial){this->serial = serial;}
