#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial GPS_Serial(11, 10); // TX, RX

int order;
Servo rudder;

void setup(){
    Serial.begin(4800);
    GPS_Serial.begin(4800);
    order = 0;
    rudder.attach(9);
    rudder.write(90);
}

void loop(){
    while(GPS_Serial.available()){
       Serial.write(GPS_Serial.read());
    }
    while(Serial.available()){
        order = (Serial.read() * 180) / 127;
        rudder.write(order);
    }
    delay(10);
}
