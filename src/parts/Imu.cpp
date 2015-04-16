#include "Imu.h"

using namespace std;

Imu::Imu() : ComThread(){
	imu_thx = 0.;  imu_thy = 0.;  imu_thz = 0.;
	imu_vthx = 0.; imu_vthy = 0.; imu_vthz = 0.;
}

Imu::~Imu(){}

void Imu::On_start(){
	std::map <int, char> keys;
	keys[4] = 115;
	keys[5] = 110;
	keys[6] = 112;
	i2c->Subscribe(keys, &(Imu::Generate_YPR), (void*) this);
}

void Imu::IO(){
	Link_output("imu_thx", &imu_thx);   Link_output("imu_thy", &imu_thy);   Link_output("imu_thz", &imu_thz);
	Link_output("imu_vthx", &imu_vthx); Link_output("imu_vthy", &imu_vthy); Link_output("imu_vthz", &imu_vthz);
}

void Imu::Job(){
	#if defined ENABLE_SERIAL && defined ENABLE_IMU
		unsigned char imu_request[8];
		imu_request[0] = 0x62; // serial
		imu_request[1] = 0x73; // s
		imu_request[2] = 0x6E; // n
		imu_request[3] = 0x70; // p
		imu_request[4] = 0x01; // PT
		imu_request[5] = 0x00; // N
		imu_request[6] = 0x01; //
		imu_request[7] = 0x52; // dates
		i2c->I2C_write(imu_request, 8);
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

void Imu::Set_i2c(I2C* i2c){this->i2c = i2c;}
