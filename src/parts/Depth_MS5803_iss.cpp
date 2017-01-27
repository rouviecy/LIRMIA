#include "Depth_MS5803_iss.h"

using namespace std;

Depth_MS5803_iss::Depth_MS5803_iss() : ComThread(){
	depth = 0.;
	enable_iss = false;
        enable_i2c = false;
        request_depth_iss_w[0] = 0x54;
        request_depth_iss_w[1] = 0xEE;
        request_depth_iss_w[2] = 0x01;
        request_depth_iss_r[0] = 0x54;
	request_depth_iss_r[1] = 0xEF;
	request_depth_iss_r[2] = 0x03;
	request_depth_iss_cr[0] = 0x54;
        request_depth_iss_cr[1] = 0xEF;
        request_depth_iss_cr[2] = 0x02;
        request_depth_i2c[0] = 0x00;
	buf[0] = 0.; buf[1] = 0.; buf[2] = 0.; buf[3] = 0.; buf[4] = 0.; buf[5] = 0.;
	Ca[0] = 0.; Ca[1] = 0.; Ca[2] = 0.; Ca[3] = 0.; Ca[4] = 0.; Ca[5] = 0.;
}

Depth_MS5803_iss::~Depth_MS5803_iss(){}

void Depth_MS5803_iss::On_start(){}

void Depth_MS5803_iss::IO(){
	Link_input("t",		COMFLOAT, 1, &t);
	Link_output("depth",	COMFLOAT, 1, &depth);
}

void Depth_MS5803_iss::Job(){
        #ifdef ENABLE_SERIAL_ISS
                if(enable_iss){
                        serial->Lock();
//calibration
			request_depth_iss_w[3] = 0x1E;
			serial->Serial_write(request_depth_iss_w, 4);

			for (int i=0; i<6  ; i++){
				request_depth_iss_w[3] = 0xA2 + (i * 2);
				serial->Serial_write(request_depth_iss_w, 4);
				Sleep(5);
				serial->Serial_write(request_depth_iss_cr, 3);
				Sleep(5)
				unsigned char* buf[i] = (unsigned char*) serial->Serial_read();
//cout << buf[0] << buf[1] << "\t" << buf[2] << "\t" << buf[3] << "\t" << buf[4] << "\t" << buf[5] << "\t" << buf[6] << "\t" << buf[7] << endl;
				long V;
				V=  (buf[1]<< 8) | buf[2];
				if(i==0){V=  (buf[2]<< 8) | buf[3];}
// cout << "V: " << V << endl;
				Ca[i+1]=V;
			}
//Get data
			request_depth_iss_w[3] = 0x42;      //Convert D1 (OSR = 512)
                        serial->Serial_write(request_depth_iss_w, 4);
			request_depth_iss_w[3] = 0x00;
                        serial->Serial_write(request_depth_iss_w, 4);
			serial->Serial_write(request_depth_iss_r, 3);
			unsigned char* d1 = (unsigned char*) serial->Serial_read();

			request_depth_iss_w[3] = 0x52;      //Convert D2 (OSR = 512)
                        serial->Serial_write(request_depth_iss_w, 4);
                        request_depth_iss_w[3] = 0x00;
                        serial->Serial_write(request_depth_iss_w, 4);
			serial->Serial_write(request_depth_iss_r, 3);
                        unsigned char* d2 = (unsigned char*) serial->Serial_read();

			dt=d2-Ca[5]*256;
			tem=2000+ dt*Ca[6]/8388608;
			off=Ca[2]*65536;
			off=off + (Ca[4]*dt)/128;
			sens=Ca[1]*32768;
			sens=sens + (Ca[3]*dt)/256;
			if (tem <= 2000for (int i=0; i<6  ; i++) {
				t2 = 3*(dt*dt);
				t2=t2/ 8589934592;
				off2 = 3*(tem-2000)*(tem-2000)/2;
				sens2 = 5*(tem-2000)*(tem-2000)/8;
			}
			else {
				t2=7*(dt*dt);
				t2=t2/137438953472;
				off2 = 1*(tem-2000)*(tem-2000)/16;
				sens2=0;
			}
			if (tem < -1500) {
				off2=off2 + 7*((tem+1500)*(tem+1500));
				sens2 =sens2 + 4*((tem+1500)*(tem+1500));
			}
			tem = tem - t2;
			off = off - off2;
			sens = sens - sens2;
			P=(d1*sens/2097152 - off)/32768;
			Pres = (float )P/ 10;
			tempe = tem/100;

			if(tim<1){zini=(Pres*100)/9810;}

			if(d1==0 || d2==0){z=z;}
			else  { z=(Pres*100)/9810-zini;}
			depth = z;
                        serial->Unlock();
                        Critical_send();
                }
        #endif

        #ifdef ENABLE_I2C
                if(enable_i2c){
                        i2c->Lock();
                        i2c->I2C_connect_device(DEPTH_MS5803_ISS_ADDRESS);
                        i2c->I2C_write(request_compass_i2c, 1);
                        unsigned char* answer = i2c->I2C_read_uchar(21);

                        i2c->Unlock();
                        Critical_send();
                }
        #endif

}

void Depth_MS5803_iss::Set_iss(Serial* serial){
        this->serial = serial;
        enable_iss = true;

}

void Depth_MS5803_iss::Set_i2c(I2C* i2c){
        #ifndef ENABLE_I2C
                cout << "[Warning] Trying to use MS5803 without I2C enabled ; pressure sensor will bew disabled" << endl;
                return;
        #endif
        this->i2c = i2c;
        enable_i2c = true;
}

