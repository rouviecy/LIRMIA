#include <Wire.h>
#include "MS5837.h"

MS5837 sensor;
float z_init;
double z_ant;

void setup() {
  
  Serial.begin(9600);
  
  //Serial.println("Starting");
  
  Wire.begin();

  sensor.init();
  
  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
  
  //z_init = sensor.depth();
  z_init = -2.4;
  z_ant = 0;
}

void loop() {

  sensor.read();

  sensor.pressure(); 
  sensor.temperature();

  double z = sensor.depth() - z_init;
  if(abs(z-z_ant)<1)
  {z = z;}
  else
  {z=z_ant;}
  
  z_ant = z; 
  //float z = sensor.depth(); 
  
  Serial.print("%DEP=");
  Serial.print(z);
  Serial.println("$");

  //delay(1000);
  delay(500);
  
  //Serial.flush();
}
