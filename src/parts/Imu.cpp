#include "Imu.h"

using namespace std;

Imu::Imu() : ComThread(){
	imu_thx = 0.;  imu_thy = 0.;  imu_thz = 0.;
	imu_vthx = 0.; imu_vthy = 0.; imu_vthz = 0.;
}

Imu::~Imu(){}

void Imu::On_start(){
	#if defined ENABLE_SERIAL && defined ENABLE_IMU
	unsigned char request[3];
	request[0] = '1';
	request[1] = 'o';
	request[2] = 't';
	serial->Serial_write(request, 1);
/*		unsigned char imu_request[8];
		imu_request[0]=0x62;// serial
		imu_request[1]=0x73;// 's'
		imu_request[2]=0x6E;// 'n'
		imu_request[3]=0x70;// 'p'
		imu_request[4]=0x48;// packet type : 1 batch
		imu_request[5]=0xB1;// reset factory settings
		imu_request[6]=0x02;// checksum high
		imu_request[7]=0x4A;// checksum low
		serial->Serial_write(imu_request, 8);
		usleep(1000000);
		imu_request[0]=0x62;// serial
		imu_request[1]=0x73;// 's'
		imu_request[2]=0x6E;// 'n'
		imu_request[3]=0x70;// 'p'
		imu_request[4]=0x48;// packet type : 1 batch
		imu_request[5]=0xAC;// gyro calibration (do not move during 3s !)
		imu_request[6]=0x02;// checksum high
		imu_request[7]=0x45;// checksum low
		serial->Serial_write(imu_request, 8);
		usleep(10000000);
*/
		
	#endif
}

void Imu::IO(){
	Link_output("imu_thx", &imu_thx);   Link_output("imu_thy", &imu_thy);   Link_output("imu_thz", &imu_thz);
	Link_output("imu_vthx", &imu_vthx); Link_output("imu_vthy", &imu_vthy); Link_output("imu_vthz", &imu_vthz);
}

void Imu::Job(){
	#if defined ENABLE_SERIAL && defined ENABLE_IMU
		while(true){
			usleep(100000);
char* answer = serial->Serial_read();
for(int i = 0; i < 128; i++){
cout << (int) answer[i] << " | ";
}
cout << endl;
			
/*			unsigned char imu_request[8];
	imu_request[0]=0x62;//instrucción para serial.
	imu_request[1]=0x73;//s
	imu_request[2]=0x6E;//n
	imu_request[3]=0x70;//p
	imu_request[4]=0x01;//PT
	imu_request[5]=0x00;//N
	imu_request[6]=0x01;//
	imu_request[7]=0x52;// para comprobar datos
	

		imu_request[0]=0x62;// serial
		imu_request[1]=0x73;// 's'
		imu_request[2]=0x6E;// 'n'
		imu_request[3]=0x70;// 'p'
		imu_request[4]=0x48;// packet type : 1 batch
		imu_request[5]=0x64;// request quaternion AB
		imu_request[6]=0x01;// checksum high
		imu_request[7]=0xFD;// checksum low
			serial->Serial_write(imu_request, 8);
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
				if(msg.size() == 4){
cout << (int) answer[4] << endl;
					if(true){msg.push_back(answer[i]);}
					else			{msg.clear();}
					continue;
				}
				msg.push_back(answer[i]);
				if(msg.size() == 15){
for(int i = 0; i < 15; i++){
cout << (int) msg[i] << " | ";
}
cout << endl;
					if(msg[4] == 94){
						quaternion_coded[2] = (msg[5] << 8) | msg[6];
						quaternion_coded[3] = (msg[7] << 8) | msg[8];
						quaternion_candidate[2] = static_cast <float> (quaternion_coded[2]) * FAC_QUATERNION_IMU;
						quaternion_candidate[3] = static_cast <float> (quaternion_coded[3]) * FAC_QUATERNION_IMU;
						if(fabs(quaternion_candidate[2]) > 0.01){q[2] = quaternion_candidate[2];}
						if(fabs(quaternion_candidate[3]) > 0.01){q[3] = quaternion_candidate[3];}
						short pitch1	= (msg[5] << 8) | msg[6];
						short roll1	= (msg[7] << 8) | msg[8];
						float candidate_pitch	= static_cast <float> (pitch1) * FAC_ANG_IMU;
						float candidate_roll	= static_cast <float> (roll1) * FAC_ANG_IMU;
						if(fabs(candidate_pitch) > 2.){PITCH = candidate_pitch;}
						if(fabs(candidate_roll) > 2.){ROLL = candidate_roll;}

					}
					else if(msg[4] == 98){
						quaternion_coded[0] = (msg[5] << 8) | msg[6];
						quaternion_coded[1] = (msg[7] << 8) | msg[8];
						quaternion_candidate[0] = static_cast <float> (quaternion_coded[0]) * FAC_QUATERNION_IMU;
						quaternion_candidate[1] = static_cast <float> (quaternion_coded[1]) * FAC_QUATERNION_IMU;
						if(fabs(quaternion_candidate[0]) > 0.01){q[0] = quaternion_candidate[0];}
						if(fabs(quaternion_candidate[1]) > 0.01){q[1] = quaternion_candidate[1];}

						short yaw1	= (msg[5] << 8) | msg[6];
						float candidate_yaw	= static_cast <float> (yaw1) * FAC_ANG_IMU;
						if(fabs(candidate_yaw) > 4.){YAW = candidate_yaw;}

					}
					cout << q[0] << "\t" << q[1] << "\t" << q[2] << "\t" << q[3] << endl;
float q01 = q[0] * q[1];
float q02 = q[0] * q[2];
float q03 = q[0] * q[3];
float q11 = q[1] * q[1];
float q12 = q[1] * q[2];
float q13 = q[1] * q[3];
float q22 = q[2] * q[2];
float q23 = q[2] * q[3];
float q33 = q[3] * q[3];

ROLL = atan2(2 * (q01 + q23), 1 - 2 * (q11 + q22)) * 57.29578;
PITCH = asin(2 * (q02 - q13)) * 57.29578;
YAW = atan2(2 * (q03 + q12), 1 - 2 * (q22 + q33)) * 57.29578;
					cout << YAW << "\t" << PITCH << "\t" << ROLL << endl;
					msg.clear();
				}
			}
*/		}

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
