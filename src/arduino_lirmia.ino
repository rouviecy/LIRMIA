#include <SoftwareSerial.h>
#include <Wire.h>

#define depth_address 0x77
#define depth_reset   0x1E
#define depth_calib   0xA2
#define depth_data    0x00

SoftwareSerial IMU_Serial(10, 11); // RX, TX

float z_init;
unsigned int calib_depth[6];

void calibrate_depth_sensor(){
    Wire.beginTransmission(depth_address);
    Wire.write(depth_reset);
    Wire.endTransmission();
    delay(5);
    for(int i = 0; i < 6; i++){
        Wire.beginTransmission(depth_address);
        Wire.write(depth_calib + 2 * i);
        Wire.endTransmission();
        Wire.requestFrom(depth_address, 2);
        byte prom_high = Wire.read();
        byte prom_low = Wire.read();
        calib_depth[i] = (unsigned int) (prom_high << 8) + (unsigned int) prom_low;
        delay(5);
    }
}

unsigned long read_depth_sensor(bool index){
    Wire.beginTransmission(depth_address);
    if(index){Wire.write(0x50);}
    else{Wire.write(0x40);}
    Wire.endTransmission();
    delay(50);
    Wire.beginTransmission(depth_address);
    Wire.write(depth_data);
    Wire.endTransmission();
    Wire.requestFrom(depth_address, 8);
    byte adc[4];
    unsigned long result = 0;
    for(int i = 0; i < 4; i++){
      adc[i] = Wire.read();
      if(!index){
        if(i > 0){result += ((unsigned long) adc[i]) << (8 * (3 - i));}
      }
      else{
        if(i < 3){result += ((unsigned long) adc[i]) << (8 * (2 - i));}
      }
    }
    delay(5);
    return result;
}

float compute_depth(){
    signed long P = 0;
    while(P < 10000){
        unsigned long data1 = read_depth_sensor(false);
        unsigned long data2 = read_depth_sensor(true);
        signed long dt = (data2 - calib_depth[4] << 8);
        signed long tem = 2000 + dt * calib_depth[5] / 8388608;
        int64_t off  = (calib_depth[1] << 17) + (calib_depth[3] * dt) >> 6;
        int64_t sens = (calib_depth[0] << 16) + (calib_depth[2] * dt) >> 7;
        P = (data1 * (signed long) (sens >> 21) - (signed long) off) >> 15;
        if(tem < 2000){
            tem = tem - (dt * dt) >> 31;
            off = off - (61 * (tem - 2000) * (tem - 2000)) >> 4;
            sens = sens - 2 * (tem - 2000) * (tem - 2000);
            P = (data1 * (signed long) (sens >> 21) - (signed long) off) >> 15;
        }
    }
    return (float) P / 9810.;
}

void setup(){
    Serial.begin(57600);
    IMU_Serial.begin(57600);
//    Wire.begin();
//    calibrate_depth_sensor();
//    z_init = compute_depth();
}

void loop(){
    int i = 0;
    while(IMU_Serial.available()){
      char new_char = IMU_Serial.read();
      if(new_char == '#'){i++;}
      if(i < 2){
        Serial.write(new_char);
      }
    }
    Serial.println("");
//    float z = compute_depth() - z_init;
//    Serial.print("#DEP=");
//    Serial.print(z);
//    Serial.println(",");
    delay(200);
}

