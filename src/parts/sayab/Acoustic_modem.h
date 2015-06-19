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

enum state_com_t {
	IDLE
};

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
	bool sub_is_underwater;
	int fsm_state;

	// Acoustic communication
	std::thread thr_reception;
	bool receive_go_on;
	std::queue <char> input_flow;
	void Get_acoustic_msg_loop();
	void Send_acoustic_msg(std::string msg);

};

#endif
