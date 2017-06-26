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

#include "../../core/ComThread.h"
#include "../../interfaces/TCP_server.h"

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
	float ti;
	float xy[2];
	float lat;
	float lon;
	float thz;
	float vthz;
	float yawref;
	float gpsreflat;
	float gpsreflon;
	float distance;
	float uw;
	float uf;
	float motor;
	float rudder;
	float bow_thruster;

//	float msg_parse;

	int fsm_state;
	bool fsm_unlocked;

};

#endif
