#include <Wire.h>
#include "MS5837.h"

MS5837 sensor;
float z_init;

void setup() {
  
  Serial.begin(9600);
  
  //Serial.println("Starting");
  
  Wire.begin();

  sensor.init();
  
  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
  
  //z_init = sensor.depth();
}

void loop() {

  sensor.read();

  sensor.pressure(); 
  sensor.temperature();
  
  //float z = sensor.depth() - z_init;
  float z = sensor.depth(); 
  
  Serial.print("%DEP=");
  Serial.print(z);
  Serial.println("$");

  //delay(1000);
  delay(200);
  
  //Serial.flush();
}
