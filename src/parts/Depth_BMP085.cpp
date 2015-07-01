#include "Depth_BMP085.h"

using namespace std;

Depth_BMP085::Depth_BMP085() : ComThread(){
	depth = 0.;
}

Depth_BMP085::~Depth_BMP085(){}

void Depth_BMP085::On_start(){Calibrate();}

void Depth_BMP085::IO(){
	Link_input("raz_depth",	COMBOOL,	1, &raz_depth);
	Link_output("depth",	COMFLOAT,	1, &depth);
}

void Depth_BMP085::Job(){
	#ifndef ENABLE_I2C
		return;
	#endif
	Critical_receive();
	if(raz_depth){Calibrate();}
	i2c->Lock();
	i2c->I2C_connect_device(BMP085_I2C_ADDRESS);
	Update_temperature();
	Update_pressure();
	Update_altitude();
	i2c->Unlock();
	Critical_send();
}

void Depth_BMP085::Calibrate(){
	#ifndef ENABLE_I2C
		return;
	#endif
	i2c->Lock();
	i2c->I2C_connect_device(BMP085_I2C_ADDRESS);
	ac1 = (short int)		Read_ushort_and_swap(0xAA);
	ac2 = (short int)		Read_ushort_and_swap(0xAC);
	ac3 = (short int)		Read_ushort_and_swap(0xAE);
	ac4 = (unsigned short int)	Read_ushort_and_swap(0xB0);
	ac5 = (unsigned short int)	Read_ushort_and_swap(0xB2);
	ac6 = (unsigned short int)	Read_ushort_and_swap(0xB4);
	b1  = (short int)		Read_ushort_and_swap(0xB6);
	b2  = (short int)		Read_ushort_and_swap(0xB8);
	mb  = (short int)		Read_ushort_and_swap(0xBA);
	mc  = (short int)		Read_ushort_and_swap(0xBC);
	md  = (short int)		Read_ushort_and_swap(0xBE);
	Update_temperature();
	Update_pressure();
	pressure_ref = pressure;
	i2c->Unlock();
}

void Depth_BMP085::Update_temperature(){
	i2c->I2C_write_ushort_address(0xF4, 0x2E);
	usleep(5000);
	unsigned short answer = Read_ushort_and_swap(0xF6);
	int x1 = (((int) answer - (int) ac6) * (int) ac5) >> 15;
	int x2 = ((int) mc << 11) / (x1 + md);
	b5 = x1 + x2;
	temperature = ((b5 + 8) >> 4);
}

void Depth_BMP085::Update_pressure(){
	i2c->I2C_write_ushort_address(0xF4, 0x34 + (BMP085_OVERSAMPLING_SETTING << 6));
	usleep((2 + (3 << BMP085_OVERSAMPLING_SETTING)) * 1000);
	unsigned char* answer = i2c->I2C_read_uchar_address(0xF6, 3);
	unsigned int Up = (((unsigned int) answer[0] << 16) | ((unsigned int) answer[1] << 8) | (unsigned int) answer[2]) >> (8 - BMP085_OVERSAMPLING_SETTING);
	int b6 = b5 - 4000;
	int x1 = (b2 * (b6 * b6) >> 12) >> 11;
	int x2 = (ac2 * b6) >> 11;
	int x3 = x1 + x2;
	int b3 = (((((int) ac1) * 4 + x3) << BMP085_OVERSAMPLING_SETTING) + 2) >> 2;
	x1 = (ac3 * b6) >> 13;
	x2 = (b1 * ((b6 * b6) >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	unsigned int b4 = (ac4 * (unsigned int) (x3 + 32768)) >> 15;
	unsigned int b7 = ((unsigned int) (Up - b3) * (50000 >> BMP085_OVERSAMPLING_SETTING));
	if(b7 < 0x80000000){
		pressure = (b7 << 1) / b4;
		return;
	}
	else{
		pressure = (b7 / b4) << 1;
		x1 = pow(pressure >> 8, 2);
		x1 = (x1 * 3038) >> 16;
		x2 = (-7357 * pressure) >> 16;
		pressure += (x1 + x2 + 3791) >> 4;
	}
}

void Depth_BMP085::Update_altitude(){
//	depth = (44330. * (1.0 - pow((float) pressure / 101325., 0.1903))) - 2.;
	depth = ((float) pressure - (float) pressure_ref) / 9810.;
cout << "Pressure : " << pressure << " | Depth : " << depth << endl;
}

unsigned short Depth_BMP085::Read_ushort_and_swap(int address){
	unsigned short answer = *(i2c->I2C_read_ushort_address(address));
	return ((answer << 8) & 0xFF00) | ((answer >> 8) & 0xFF);
}

void Depth_BMP085::Set_i2c(I2C* i2c){
	#ifndef ENABLE_I2C
		cout << "[Warning] Trying to use BMP085 without I2C enabled ; depth sensor will be disabled" << endl;
		return;
	#endif
	this->i2c = i2c;
}
