/*
 * @(#)		Subscriber.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Subscribe to serial communication
 */

#ifndef SUBSCRIBER
#define SUBSCRIBER

#include "../core/ComThread.h"
#include "../interfaces/Serial.h"
#include <map>
#include <vector>

#define SUBSCRIBER_BUFFER_SIZE	1024

class Subscriber : public ComThread{

public:

        Subscriber();
        ~Subscriber();

        void On_start();
        void IO();
        void Job();

        void Set_serial(Serial* serial);
        void Subscribe(std::map <int, char> keys, void (*callback) (void*, char*), void* obj);
        void Check_msg(char* msg);

        void Lock();
        void Unlock();

private:

	typedef struct{
		std::map <int, char> keys;
		void (*callback) (void*, char*);
		void* obj;
	}subscription;

	Serial *serial;

	char buffer[SUBSCRIBER_BUFFER_SIZE];
	int buffer_pos;

	std::vector <subscription> subscriptions;

};

#endif
