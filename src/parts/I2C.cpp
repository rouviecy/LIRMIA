#include "I2C.h"

using namespace std;

I2C::I2C() : ComThread(){
	#if defined(ENABLE_I2C) and not defined(ENABLE_SERIAL_ISS)
		cout << "[Warning] You are trying to use i2c without serial : i2c will be disabled" << endl;
	#endif
}

I2C::~I2C(){}

void I2C::On_start(){}

void I2C::IO(){}

void I2C::Subscribe(map <int, char> keys, void (*callback) (void*, char*), void* obj){
	subscription new_subscription;
	new_subscription.keys = keys;
	new_subscription.callback = callback;
	new_subscription.obj = obj;
	subscriptions.push_back(new_subscription);
}

void I2C::Job(){
	#if defined(ENABLE_I2C) and defined(ENABLE_SERIAL_ISS)
		char* msg = serial->Serial_read();
		for(size_t i = 0; i < subscriptions.size(); i++){
			bool selected = true;
			for(map <int, char> ::iterator it = subscriptions[i].keys.begin(); it != subscriptions[i].keys.end(); ++it){
				if(msg[it->first] != it->second){
					selected = false;
					break;
				}
			}
			if(selected){
				serial->Lock();
				subscriptions[i].callback(subscriptions[i].obj, msg);
				serial->Unlock();
			}
		}
	#endif
}

void I2C::I2C_write(unsigned char* msg, int len){serial->Serial_write(msg, len);}

char* I2C::I2C_read(){return serial->Serial_read();}

void I2C::Lock(){serial->Lock();}
void I2C::Unlock(){serial->Unlock();}

void I2C::Set_serial(Serial* serial){this->serial = serial;}
