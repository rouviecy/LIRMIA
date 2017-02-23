#include "Depth_MS5803.h"

using namespace std;

Depth_MS5803::Depth_MS5803() : ComThread(){
	depth = 0.;
	raz_depth = true;
        request_depth_iss_w[0] = 0x55; //54(77)
        request_depth_iss_w[1] = 0xEC; //EE(77)
        request_depth_iss_w[2] = 0x01;
        request_depth_iss_r[0] = 0x55;
	request_depth_iss_r[1] = 0xED; //EF(77)
	request_depth_iss_r[2] = 0x03;
	request_depth_iss_cr[0] = 0x55;
        request_depth_iss_cr[1] = 0xED; //EF(77)
        request_depth_iss_cr[2] = 0x02;
	buf[0] = 0.; buf[1] = 0.; buf[2] = 0.; buf[3] = 0.; buf[4] = 0.; buf[5] = 0.;
	Ca[0] = 0.; Ca[1] = 0.; Ca[2] = 0.; Ca[3] = 0.; Ca[4] = 0.; Ca[5] = 0.;
}

Depth_MS5803::~Depth_MS5803(){}

void Depth_MS5803::On_start(){}

void Depth_MS5803::IO(){
	Link_input("raz_depth",	COMBOOL,  1, &raz_depth);
	Link_input("t",		COMFLOAT, 1, &t);
	Link_output("depth",	COMFLOAT, 1, &depth);
}

void Depth_MS5803::Job(){
	#ifndef ENABLE_SERIAL_ISS_DEPTH
		cout << "[Warning] Trying to use MS5803 without ISS enabled ; depth sensor will be disabled" << endl;
		return;
	#endif

        if(raz_depth){Calibrate();}
	serial->Lock();

	request_depth_iss_w[3] = 0x42;      //Convert D1 (OSR = 512)
	serial->Serial_write(request_depth_iss_w, 4);
	request_depth_iss_w[3] = 0x00;
	serial->Serial_write(request_depth_iss_w, 4);
	serial->Serial_write(request_depth_iss_r, 3);
	unsigned char* answer = (unsigned char*) serial->Serial_read();
	unsigned long d1 = answer[2] * (unsigned long) 65536 + answer[3] * (unsigned long) 256 + answer[4];

	request_depth_iss_w[3] = 0x52;      //Convert D2 (OSR = 512)
	serial->Serial_write(request_depth_iss_w, 4);
	request_depth_iss_w[3] = 0x00;
	serial->Serial_write(request_depth_iss_w, 4);
	serial->Serial_write(request_depth_iss_r, 3);
	answer = (unsigned char*) serial->Serial_read();
	unsigned long d2 = answer[2] * (unsigned long) 65536 + answer[3] * (unsigned long) 256 + answer[4];

	t=d2-Ca[5]*256;
	tem=2000+ dt*Ca[6]/8388608;
	off=Ca[2]*65536;
	off=off + (Ca[4]*dt)/128;
	sens=Ca[1]*32768;
	sens=sens + (Ca[3]*dt)/256;
	if (tem <= 2000){
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

	if(raz_depth){zini=(Pres*100)/9810;}

	if(d1==0 || d2==0){z=z;}
	else  { z=(Pres*100)/9810-zini;}
	depth = z;

	serial->Unlock();
	Critical_send();
}

void Depth_MS5803::Calibrate(){
	#ifndef ENABLE_SERIAL_ISS_DEPTH
		return;
	#endif
	serial->Lock();
	request_depth_iss_w[3] = 0x1E;  //ADC Reset command
	serial->Serial_write(request_depth_iss_w, 4);
	for(int i = 0; i < 6; i++){
		request_depth_iss_w[3] = 0xA2 + (i * 2);
		serial->Serial_write(request_depth_iss_w, 4);
		usleep(5);
		serial->Serial_write(request_depth_iss_cr, 3);
		usleep(5);
		unsigned char* buf = (unsigned char*) serial->Serial_read();
cout << "buf: " << buf[0] << "\t" << buf[1] << "\t" << buf[2] << "\t" << buf[3] << "\t" << buf[4] << "\t" << buf[5] << "\t" << buf[6] << "\t" << buf[7] << endl;
		long V;
		V = i == 0 ? (buf[2] << 8) | buf[3] : (buf[1] << 8) | buf[2];
cout << "V: " << V << endl;
		Ca[i + 1] = V;
	}
}

void Depth_MS5803::Set_iss(Serial* serial){
        this->serial = serial;
}
