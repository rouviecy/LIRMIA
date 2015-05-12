#include "Depth.h"

using namespace std;

Depth::Depth() : ComThread(){
	depth = 0.;
	z_init = 0;
	#if defined(ENABLE_DEPTH) and (not defined(ENABLE_I2C) or not defined(ENABLE_SERIAL_ISS))
		cout << "[Warning] You are trying to use depth sensor without serial and i2c enabled : depth sensor will be disabled" << endl;
	#endif
}

Depth::~Depth(){}

void Depth::On_start(){}

void Depth::IO(){
	Link_input("t",		COMFLOAT, 1, &t);
	Link_output("depth",	COMFLOAT, 1, &depth);
}

void Depth::Job(){
	Critical_receive();
	#if defined(ENABLE_DEPTH) and defined(ENABLE_I2C) and defined(ENABLE_SERIAL_ISS)
// TODO : add check keys and subscribe to i2c
/*		unsigned char request_depth1[2]	= {0x42, 0x00};
		unsigned char request_depth2[2]	= {0x52, 0x00};
		i2c->Lock();
		i2c->I2C_write(request_depth1, 1);
		int answer1 = Read(0);
		i2c->I2C_write(request_depth2, 1);
		int answer2 = Read(1);
		i2c->Unlock();

		float dt = (float) (answer2 - calib_params[4] << 8);
		float tem = 2000. + dt * (float) calib_params[5] / 8388608.;
		float off  = ((float) (calib_params[1] << 16) * 4. + ((float) calib_params[3] * dt)) / 32.;
		float sens = ((float) (calib_params[0] << 16) * 2. + ((float) calib_params[2] * dt)) / 128.;
		float P = ((float) answer1 * sens / 2097152. - off) / 32768.;

		if(tem > 2000.){
			P = P / 100;
			tem = tem / 100;
		}
		else{
			float t2 = 3. * (dt * dt) / 8589934592.;
			float tem2_sqr = (tem - 2000.) * (tem - 2000.);
			float off2 = 3. * tem2_sqr / 8.;
			float sens2 = 7. * tem2_sqr;
			tem = tem - t2;
			off = off - off2;
			sens = sens - sens2;
			P = ((float) answer1 * sens / 2097152. - off) / 32768.;
			P = P / 100;
			tem = tem / 100;
		}

		if(t < 1.){z_init = (P * 100.) / 9810.;}
		depth = (P * 100.) / 9810. - z_init;
		Critical_send();
*/	#endif
}

int Depth::Read(int index){
	unsigned char resquest[3];
	resquest[0] = 0x54;
	resquest[1] = 0xEF;
	resquest[2] = 0x03;
	i2c->I2C_write(request_depth2, 1);
	unsigned char* answer = (unsigned char*) i2c->I2C_read();
	if(index == 0){return answer[3] << 16 | buf[4] << 8 | buf[5];}
	if(index == 1){return answer[2] << 16 | buf[3] << 8 | buf[4];}
	return 0;
}

void Depth::Calibrate(){
	#if defined(ENABLE_DEPTH) and defined(ENABLE_I2C) and defined(ENABLE_SERIAL_ISS)
		unsigned char resquest_calibration[4];
		resquest_calibration[0] = 0x54;
		resquest_calibration[1] = 0xEE;
		resquest_calibration[2] = 0x01;
		resquest_calibration[3] = 0x1E;
		i2c->Lock();
		i2c->I2C_write(resquest_calibration, 4);
		usleep(5000);
		for(int i = 0; i < 6; i++){
			resquest_calibration[1] = 0xEE;
			resquest_calibration[2] = 0x01;
			resquest_calibration[3] = 0xA2 + (i * 2);
			i2c->I2C_write(resquest_calibration, 4);
			usleep(5000);
			resquest_calibration[1] = 0xEF;
			resquest_calibration[2] = 0x02;
			i2c->I2C_write(resquest_calibration, 3);
			usleep(5000);
			unsigned char* answer_calibration = (unsigned char*) i2c->I2C_read();
			if(i == 0)	{calib_params[i] = (int) ((answer_calibration[4] << 8) | answer_calibration[5]);}
			else		{calib_params[i] = (int) ((answer_calibration[1] << 8) | answer_calibration[2]);}
cout << calib_params[i] << endl;
			usleep(5000);
		}
		i2c->Unlock();
	#endif
}

void Depth::Set_i2c(I2C* i2c){this->i2c = i2c;}
