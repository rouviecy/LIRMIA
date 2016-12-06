#include "Subscriber.h"

using namespace std;

Subscriber::Subscriber() : ComThread(){
	buffer_pos = 0;
}

Subscriber::~Subscriber(){}

void Subscriber::On_start(){}

void Subscriber::IO(){}

void Subscriber::Subscribe(map <int, char> keys, void (*callback) (void*, char*), void* obj){
	#ifndef ENABLE_SERIAL_ARDUINO
		cout << "[Error] Serial needs to be enabled to use Subscriber" << endl;
		return;
	#endif
	//#ifndef ENABLE_SERIAL_RS232_IMU
        //        cout << "[Error] Serial needs to be enabled to use Subscriber" << endl;
        //        return;
        //#endif

	subscription new_subscription;
	new_subscription.keys = keys;
	new_subscription.callback = callback;
	new_subscription.obj = obj;
	subscriptions.push_back(new_subscription);
}

void Subscriber::Job(){
	#ifndef ENABLE_SERIAL_ARDUINO
                return;
        #endif
        //#ifndef ENABLE_SERIAL_RS232_IMU
        //        return;
        //#endif
        char* msg = serial->Serial_read();
        while(*msg != 0 && buffer_pos < SUBSCRIBER_BUFFER_SIZE){
                if(*msg != '\n'){
                        buffer[buffer_pos] = *msg;
                        buffer_pos++;
                }
                else{
                        Check_msg(buffer);
			buffer[buffer_pos] = 0;
                        buffer_pos = 0;
                }
                msg += sizeof(char);
        }
}

void Subscriber::Check_msg(char* msg){
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
}

void Subscriber::Lock(){serial->Lock();}
void Subscriber::Unlock(){serial->Unlock();}

void Subscriber::Set_serial(Serial* serial){this->serial = serial;}

