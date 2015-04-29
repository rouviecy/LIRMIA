#include <SoftwareSerial.h>

SoftwareSerial IMU_Serial(10, 11); // RX, TX

void setup(){
    Serial.begin(57600);
    IMU_Serial.begin(57600);
}

void loop(){
    if(IMU_Serial.available()){Serial.write(IMU_Serial.read());}
    if(Serial.available()){IMU_Serial.write(Serial.read());}
}

