/*
 * @(#)		Acoustic_modem.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Communicate to other robots with acoustic modem with serial
 */

#ifndef ACOUSTIC_MODEM
#define ACOUSTIC_MODEM

#include "../core/ComThread.h"
#include "../interfaces/FSM.h"
#include "../interfaces/FSMDraw.h"
#include <queue>

#define MAX_COMMANDS_IN_BUFFER			10
#define ACOUSTIC_DT_REPEAT_KEY			5.
#define ACOUSTIC_MSG_KEY			string("4242")
#define ACOUSTIC_MSG_REQUEST_XYZ		string("rxyz")
#define ACOUSTIC_MSG_REQUEST_YPR		string("rypr")
#define ACOUSTIC_MSG_COMMAND_REMOTE_ONOFF	string("rrem")
#define ACOUSTIC_MSG_COMMAND_STAY		string("rsta")
#define ACOUSTIC_MSG_COMMAND_EXPLORE		string("rexp")
#define ACOUSTIC_MSG_COMMAND_NOFOLLOW		string("rnof")
#define ACOUSTIC_MSG_COMMAND_UP			string("rup0")
#define ACOUSTIC_MSG_COMMAND_DOWN		string("rdow")
#define ACOUSTIC_HEADER_X			'x'
#define ACOUSTIC_HEADER_Y			'y'
#define ACOUSTIC_HEADER_Z			'z'
#define ACOUSTIC_HEADER_YPR			'a'
#define ACOUSTIC_HEADER_COMMAND			'c'
#define ACOUSTIC_CONFIRM_XYZ			string("txyz")
#define ACOUSTIC_CONFIRM_YPR			string("typr")
#define ACOUSTIC_CONFIRM_COMMAND		string("tcom")

enum state_com_t {
	IDLE,
	MASTER_BAD_COM, MASTER_GOOD_COM, MASTER_WANT_X, MASTER_WANT_Y, MASTER_WANT_Z, MASTER_WANT_YPR, MASTER_CONFIRM_XYZ, MASTER_CONFIRM_YPR, MASTER_CONFIRM_COMMAND,
	SLAVE_GOOD_COM, SLAVE_IMPROVE_ANGLE, SLAVE_IMPROVE_DISTANCE, SLAVE_SEND_X, SLAVE_SEND_Y, SLAVE_SEND_Z, SLAVE_SEND_YPR, SLAVE_EXEC_COMMAND
};

enum command_t {
	COMMAND_REMOTE_ONOFF, COMMAND_STAY, COMMAND_EXPLORE, COMMAND_NOFOLLOW, COMMAND_UP, COMMAND_DOWN
};

class Acoustic_modem : public ComThread{

public:

	Acoustic_modem();
	~Acoustic_modem();

	void Get_xyz();
	void Get_ypr();
	void Send_command(command_t command);

private:

	FSM fsm;
	FSMDraw drawer;

	// ComThread
	void On_start();
	void Job();
	void IO();

	// Inputs
	float t;
	float xyz[3];
	float thxyz[3];

	// Outputs
	int fsm_state;
	float modem_txyz[4];
	float modem_typr[4];
	bool modem_stab;
	float order_xyz_com[3];
	float order_thxyz_com[3];

	// Requests
	std::mutex mu;
	float last_t_msg;
	float tmp_xyz[3];
	float tmp_ypr[3];
	bool need_xyz;
	bool need_ypr;
	std::queue <command_t> command_buffer;

	// Acoustic communication
	std::queue <std::string> in_msg_queue;
	std::string last_msg;
	bool Compare_msg(std::string msg1, std::string msg2);
	float Decode_acoustic_msg(std::string msg, int begin, int end);
	void Process_data(char wanted_header, float* destination);
	void Get_acoustic_msg();
	void Send_acoustic_msg(std::string msg);

	// Common actions
	static void Back_to_idle			(void* obj);

	// Master actions
	static void Send_key_and_stabilize		(void* obj);
	static void Send_request_xyz			(void* obj);
	static void Send_request_ypr			(void* obj);
	static void Send_request_command		(void* obj);
	static void Send_same_msg			(void* obj);

	// Slave actions
	static void Stabilize_without_obliquity		(void* obj);
	static void Stabilize_with_little_obliquity	(void* obj);
	static void Change_precession			(void* obj);
	static void Decrease_distance			(void* obj);
	static void Send_x				(void* obj);
	static void Send_y				(void* obj);
	static void Send_z				(void* obj);
	static void Send_ypr				(void* obj);
	static void Exec_command			(void* obj);
	static void Send_confirm_msg			(void* obj);
	static void Send_wrong_msg			(void* obj);

};

#endif
