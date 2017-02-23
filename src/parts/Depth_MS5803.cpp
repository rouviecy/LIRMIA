#include "Depth_MS5803.h"

using namespace std;

Depth_MS5803::Depth_MS5803() : ComThread(){
	depth = 0.;
	raz_depth = true;
	request_depth_iss_res[0] = 0x54;
	request_depth_iss_res[1] = 0xEC;
	request_depth_iss_res[1] = 0x01;
	request_depth_iss_res[2] = 0x1E;
	Ca[1] = 65281; Ca[2] = 52481; Ca[3] = 30209; Ca[4] = 12033; Ca[5] = 17665; Ca[6] = 1281;
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
//	Critical_receive();

//      if(raz_depth){Calibrate();}
	Calibrate();
//	serial->Lock();
	Update_depth();
//	serial->Unlock();
	Critical_send();
}

void Depth_MS5803::Calibrate(){
	#ifndef ENABLE_SERIAL_ISS_DEPTH
		return;
	#endif
	serial->Lock();
	serial->Serial_write(request_depth_iss_res, 4);
	usleep(5000);
//	serial->Lock();
	for(int i = 0; i < 6; i++){
		usleep(5000);
		request_depth_iss_w[0] = 0x54;
		request_depth_iss_w[1] = 0xEC;
		request_depth_iss_w[2] = 0x01;
		request_depth_iss_w[3] = 0xA2 + (i * 2);
		serial->Serial_write(request_depth_iss_w, 4);
		usleep(5000);
		request_depth_iss_r[0] = 0x54;
		request_depth_iss_r[1] = 0xED;
		request_depth_iss_r[2] = 0x02;
		serial->Serial_write(request_depth_iss_r, 3);
		unsigned char* buf = (unsigned char*) serial->Serial_read();
//cout << "buf: " << buf[i] << endl;
		long V;
		V = i == 0 ? (buf[2] << 8) | buf[3] : (buf[1] << 8) | buf[2];
//cout << "V : " << V << endl;
		Ca[i + 1] = V;
cout << "Ca :" << Ca[i+1] << endl;
	}
	serial->Unlock();
}

void Depth_MS5803::Update_depth(){
	serial->Lock();
	serial->Serial_write(request_depth_iss_res, 4);
	usleep(5000);
	request_depth_iss_w[0] = 0x54;
	request_depth_iss_w[1] = 0xEC;  //EC(76)
	request_depth_iss_w[2] = 0x01;
	request_depth_iss_w[3] = 0x42;	//Convert D1 (42 OSR = 512)
	serial->Serial_write(request_depth_iss_w, 4);
	usleep(5000);
//	request_depth_iss_w[0] = 0x54;
//	request_depth_iss_w[1] = 0xEC;  //EC(76)
//	request_depth_iss_w[2] = 0x01;
//	request_depth_iss_w[3] = 0x00;
//	serial->Serial_write(request_depth_iss_w, 4);
//	usleep(10000);
	request_depth_iss_r[0] = 0x54;
	request_depth_iss_r[1] = 0xED;	//ED(76)
	request_depth_iss_r[2] = 0x03;
	serial->Serial_write(request_depth_iss_r, 3);
	unsigned char* answer = (unsigned char*) serial->Serial_read();
	unsigned long d1 = answer[2] * (unsigned long) 65536 + answer[3] * (unsigned long) 256 + answer[4];
cout << "d1 : " << d1 << endl;
	usleep(5000);
        request_depth_iss_w[0] = 0x54;
        request_depth_iss_w[1] = 0xEC;	//EC(76)
        request_depth_iss_w[2] = 0x01;
        request_depth_iss_w[3] = 0x52;	//Convert D2 (52 OSR = 512)
	serial->Serial_write(request_depth_iss_w, 4);
	usleep(5000);
	request_depth_iss_w[4] = 0x54;
	request_depth_iss_w[5] = 0xEC;	//EC(76)
	request_depth_iss_w[6] = 0x01;
	request_depth_iss_w[7] = 0x00;
	serial->Serial_write(request_depth_iss_w, 4);
	usleep(5000);
        request_depth_iss_r[0] = 0x54;
        request_depth_iss_r[1] = 0xED;	//ED(76)
        request_depth_iss_r[2] = 0x03;
        serial->Serial_write(request_depth_iss_r, 3);
//	unsigned char* answert = (unsigned char*) serial->Serial_read();
//	unsigned long d2 = answert[2] * (unsigned long) 65536 + answert[3] * (unsigned long) 256 + answert[4];
	unsigned long d2 = 7340289;
cout << "d2 : " << d2 << endl;
	usleep(5000);
	dt=d2-Ca[5]*256;
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
	Pres = (float)P/ 10;
	tempe = tem/100;

	if(raz_depth){zini=(Pres*100)/9810;
		raz_depth = false;
	}

	if(d1==0 || d2==0){z=z;}
	else  { z=(Pres*100)/9810-zini;}

	depth = z;
cout << " t : "<< t << " z : " << z << " tem : " << tempe << " P : " << P << " zini :  " << zini << endl;
	serial->Unlock();
}

void Depth_MS5803::Set_iss(Serial* serial){
        this->serial = serial;
}
