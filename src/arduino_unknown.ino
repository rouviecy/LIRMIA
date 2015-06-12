#include <SoftwareSerial.h>

SoftwareSerial GPS_Serial(10, 11); // RX, TX

int order;

void setup(){
    Serial.begin(4800);
    GPS_Serial.begin(4800);
    order = 0;
}

void loop(){
    while(GPS_Serial.available()){
        Serial.write(GPS_serial.read());
    }
    while(Serial.available(){
        order = (int) Serial.read();
    }
// order stuff
    delay(200);
}

