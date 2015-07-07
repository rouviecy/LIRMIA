#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial GPS_Serial(11, 10); // TX, RX

unsigned char header_rudder[5] = {'#','R','U','D','='};

int order;
Servo rudder;

void setup(){
    Serial.begin(4800);
    GPS_Serial.begin(4800);
    order = 0;
//    rudder.attach(3);
//    rudder.write(90);
}

void loop(){
    while(GPS_Serial.available()){
       Serial.write(GPS_Serial.read());
    }
/*    int pos_header = 0;
    while(Serial.available()){
        char msg_char = Serial.read();
        if(pos_header == 5){
            order = (msg_char * 180) / 127;
            rudder.write(order);
            pos_header = 0;
            continue;
	}
        if(msg_char != header_rudder[pos_header]){
            pos_header = 0;
            continue;
        }
        else{
            pos_header++;
        }
    }
*/    delay(10);
}
