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

enum state_com_t {
	IDLE,
	MASTER_BAD_COM, MASTER_GOOD_COM, MASTER_WANT_X, MASTER_WANT_Y, MASTER_WANT_Z, MASTER_WANT_YPR, MASTER_WANT_COMMAND, MASTER_CONFIRM_XYZ, MASTER_CONFIRM_YPR, MASTER_CONFIRM_COMMAND,
	SLAVE_GOOD_COM, SLAVE_IMPROVE_ANGLE, SLAVE_IMPROVE_DISTANCE, SLAVE_SEND_X, SLAVE_SEND_Y, SLAVE_SEND_Z, SLAVE_SEND_YPR, SLAVE_EXEC_COMMAND
};

class Acoustic_modem : public ComThread{

public:

	Acoustic_modem();
	~Acoustic_modem();

private:

	FSM fsm;
	FSMDraw drawer;

	void On_start();
	void Job();
	void IO();
	
	float xyz[3];
	float thxyz[3];

	int fsm_state;

	static void Back_to_idle			(void* obj);

	static void Send_key_and_stabilize		(void* obj);
	static void Send_request_xyz			(void* obj);
	static void Send_request_ypr			(void* obj);
	static void Send_request_command		(void* obj);
	static void Send_same_msg			(void* obj);

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
