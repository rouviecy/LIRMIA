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

#include "../../core/ComThread.h"
#include "../../interfaces/Serial.h"
#include <queue>

typedef struct{
	char addressee;
	char header;
	bool checksum;
	char data[3];
} MSG_MODEM;

class Acoustic_modem : public ComThread{

public:

	Acoustic_modem();
	~Acoustic_modem();

	void Set_serial(Serial* serial);
	void Stop_receive();

private:

	// Objects
	Serial* serial;

	// ComThread
	void On_start();
	void Job();
	void IO();

	// IO
	int fsm_state;
	float xy_modem[2];
	float thzm[2];

	// Acoustic communication
	std::thread thr_reception;
	bool receive_go_on;
	std::mutex mu;
	char buffer[4];
	int buffer_pos;
	std::queue <MSG_MODEM> input_flow;
	static void Get_acoustic_msg_loop(Acoustic_modem* self);
	void Send_acoustic_msg(std::string msg);

};

#endif
