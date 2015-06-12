#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial GPS_Serial(10, 11); // RX, TX

int order;
Servo rudder;

void setup(){
    Serial.begin(4800);
    GPS_Serial.begin(4800);
    order = 0;
    rudder.attach(9); // Direction servo motor is on digital pin 9
    rudder.write(90);
}

void loop(){
    while(GPS_Serial.available()){
        Serial.write(GPS_serial.read());
    }
    while(Serial.available(){
        order = (int) Serial.read() - 127; // incoming order from -127 to +127
        rudder.write(90 + (order * 90) / 127);
    }
    delay(200);
}

