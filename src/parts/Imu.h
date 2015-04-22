/*
 * @(#)		Imu.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Inertial Measurement Unit with serial
 */

#ifndef IMU
#define IMU

#include <cmath>
#include "../core/ComThread.h"
#include "../interfaces/Serial.h"

class Imu : public ComThread{

public:

	Imu();
	~Imu();

	void Set_serial(Serial* serial);

private:

	typedef struct{
		bool HasData;
		bool IsBatch;
		char BatchLength;
		bool CommFail;
		char Address;
		char Checksum1;
		char Checksum0;
		char DataLength;
	} UM6_PacketStruct;

	#define DATA_BUFF_LEN  16
	char aPacketData[DATA_BUFF_LEN];

	#define UM6_GET_DATA 0xAE
	#define UM6_QUAT_AB  0x64
	#define UM6_QUAT_CD  0x65
	#define PT_HAS_DATA  0b10000000
	#define PT_IS_BATCH  0b01000000
	#define PT_COMM_FAIL 0b00000001

	#define FAC_ANG_IMU 0.0109863	// Factor de escala de los angulos YAW, PITCH y ROLL
	#define FAC_RAT_IMU 0.0137329	// Factor de escala de YAW RATE
	#define FAC_ACC_IMU 0.15387	// Factor de escala de ACELERACIÓN
	#define FAC_QUATERNION_IMU 0.0000335693

	Serial* serial;

	void On_start();
	void Job();
	void IO();

	float imu_thx,  imu_thy,  imu_thz;
	float imu_vthx, imu_vthy, imu_vthz;

	float YAW, PITCH, ROLL;
	short quaternion_coded[4];
	float quaternion_candidate[4];
	float q[4];

	static void Generate_YPR(void* obj, char* answer);
	std::vector <char> msg;
	UM6_PacketStruct UM6_Packet;

};

#endif
