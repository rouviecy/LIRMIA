/*
 * @(#)		Remote.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Remote control with TCP
 */

#ifndef REMOTE
#define REMOTE

#include "../core/ComThread.h"
#include "../interfaces/TCP_server.h"
#include "../interfaces/vision/Blobs.h"
#include "../interfaces/vision/struct_HSV_bound.h"

class Remote : public ComThread{

public:

	Remote();
	~Remote();

	void Job_and_wait_quit();
	void Set_blobs_obj(Blobs* blobs);

private:

	TCP_server tcp_server;
	STRUCT_HSV_BOUND hsv;
	Blobs* blobs;

	void On_start();
	void Job();
	void IO();

	float remote;
	float remote_forward;
	float remote_turn;
	float remote_deeper;

	bool alive;

};

#endif
