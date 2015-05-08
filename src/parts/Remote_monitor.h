/*
 * @(#)		Remote_monitor.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Remote monitor sending data with TCP
 */

#ifndef REMOTE_MONITOR
#define REMOTE_MONITOR

#include "../core/ComThread.h"
#include "../interfaces/TCP_server.h"

class Remote_monitor : public ComThread{

public:

	Remote_monitor();
	~Remote_monitor();

private:

	TCP_server tcp_server;

	void On_start();
	void Job();
	void IO();

	float t;
	float thxyz[3];
	float motor[4];
	int fsm_state;
	bool fsm_unlocked;

};

#endif
