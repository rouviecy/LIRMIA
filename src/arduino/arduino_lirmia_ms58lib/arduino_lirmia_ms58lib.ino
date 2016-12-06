#include <SoftwareSerial.h>
#include <Wire.h>
#include <SparkFun_MS5803_I2C.h>

//  ADDRESS_HIGH = 0x76
//  ADDRESS_LOW  = 0x77

MS5803 sensor(ADDRESS_HIGH);

SoftwareSerial IMU_Serial(10, 11); // RX, TX

float z_init;
float temperature_c, temperature_f;
double pressure_abs, pressure_relative, altitude_delta, pressure_baseline;

// Create Variable to store altitude in (m) for calculations;
double base_altitude = 1655.0; // Altitude of SparkFun's HQ in Boulder, CO. in (m)

void setup(){
    Serial.begin(57600);
    //IMU_Serial.begin(57600);
    //Retrieve calibration constants for conversion math.
    sensor.reset();
    sensor.begin();
    pressure_baseline = sensor.getPressure(ADC_512);
    z_init = pressure_baseline/98.1;
}

void loop(){
    //IMU data
    //int i = 0;
    //while(IMU_Serial.available()){
      //char new_char = IMU_Serial.read();
      //if(new_char == '#'){i++;}
      //if(i < 2){
        //Serial.write(new_char);
     //}
    //}
    // Read pressure from the sensor in mbar.
    //delay (120);
      pressure_abs = sensor.getPressure(ADC_512);
      //Serial.println("");
      //Depth
      float z = pressure_abs/98.1 - z_init;
  //    delay(100);
      Serial.print("%DEP=");
      Serial.print(z);
      Serial.println("$");
      //Serial.println("");
      //    delay(100);
     delay(120); 
     Serial.flush();
}

