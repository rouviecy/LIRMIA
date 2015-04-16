/*
 * @(#)		I2C.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Provide I2C communication
 */

#ifndef I2C_H
#define I2C_H

#include "../core/ComThread.h"
#include "../interfaces/Serial.h"
#include <map>
#include <vector>

class I2C : public ComThread{

public:

	I2C();
	~I2C();
	
	void On_start();
	void IO();
	void Job();

	void Set_serial(Serial* serial);
	void Subscribe(std::map <int, char> keys, void (*callback) (void*, char*), void* obj);

	void I2C_write(unsigned char* msg, int len);
	char* I2C_read();

	void Lock();
	void Unlock();
	
private:

	typedef struct{
		std::map <int, char> keys;
		void (*callback) (void*, char*);
		void* obj;
	}subscription;

	Serial* serial;

	std::vector <subscription> subscriptions;

};

#endif
