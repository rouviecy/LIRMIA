/*
 * @(#)		Sonar.h
 * @version	1.2
 * @autor	L. Revueltas
 */
/**
 * 
 */

#ifndef SONAR
#define SONAR

#include "../core/ComThread.h"
#include "../interfaces/Serial.h"
#include "../interfaces/TCP_server.h"

class Sonar : public ComThread{

public:

	Sonar();
	~Sonar();

	void Set_serial_x(Serial* serial);
	void Set_serial_y(Serial* serial);
	void Set_serial_s(Serial* serial);

private:
	std::map <int, TCP_server*> tcp_servers;
	Serial *serial_x, *serial_y,*serial_s;

	void On_start();
	void Job();
	void IO();

	float obst_xy[2];
	unsigned char mtSendV;
	float* sData;
	float sData2[245];
	int fready;
	bool abc;

	unsigned char request;

	float Get_version(Serial* serial);
	float Get_user(Serial* serial);
	float Config_sonar(Serial* serial);
	float Ask_data(Serial* serial);
	float* Get_data(Serial* serial, float* data);
	bool Add_flux(int port);//6247
	bool Send_tcp(int* sData, int port);

};

#endif
