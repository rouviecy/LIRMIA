/*
 * @(#)		Remote_control.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Remote control receiving orders from TCP
 */

#ifndef REMOTE_CONTROL
#define REMOTE_CONTROL

#include "../../core/ComThread.h"
#include "../../interfaces/TCP_server.h"
#include "../../interfaces/vision/Blobs.h"
#include "../../interfaces/vision/HSV_tools.h"

class Remote_control : public ComThread{

public:

	Remote_control();
	~Remote_control();

	void Job_and_wait_quit();
	void Set_blobs_obj(Blobs* blobs);

private:

	TCP_server tcp_server;
	HSV_tools hsv;
	Blobs* blobs;

	void On_start();
	void Job();
	void IO();

	int fsm_state;
	bool remote;
	float remote_forward, remote_turn;
	bool fsm_unlocked, fsm_law_control, fsm_nofollow, fsm_explore, fsm_stabilize;
	bool enable_streaming;

	bool alive;

};

#endif
