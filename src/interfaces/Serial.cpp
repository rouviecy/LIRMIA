#include "Serial.h"

using namespace std;

Serial::Serial(){
	bzero(buf, SERIAL_BUFFER_LEN);
}

void Serial::Serial_init(const char* path, int baudrate, bool canonical_mode){
	this->path = string(path);
	this->baudrate = baudrate;
	this->canonical_mode = canonical_mode;
	device = open(path, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
	if(device < 0){
		cout << "[Error] Unable to open serial port to " + this->path << endl;
		return;
	}
	memset(&tio_new, 0, sizeof(tio_new));
	tcgetattr(device, &tio_old);
	cfsetispeed(&tio_new, baudrate);
	cfsetospeed(&tio_new, baudrate);
	if(canonical_mode){
		tio_new.c_cflag &= ~PARENB;
		tio_new.c_cflag &= ~CSTOPB;
		tio_new.c_cflag &= ~CSIZE;
		tio_new.c_cflag |= CS8;
		tio_new.c_cflag &= ~CRTSCTS;
		tio_new.c_cflag |= CREAD | CLOCAL;
		tio_new.c_iflag &= ~(IXON | IXOFF | IXANY);
		tio_new.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);
		tio_new.c_oflag &= ~OPOST;
		tcsetattr(device, TCSANOW, &tio_new);
		tcflush(device, TCIFLUSH);
	}
	else{
		tio_new.c_cflag |= CS8;
		tio_new.c_cflag |= CRTSCTS;
		tio_new.c_cflag |= CREAD | CLOCAL;
		tio_new.c_iflag |= IGNPAR;
		tio_new.c_cc[VTIME] = 0;
		tio_new.c_cc[VMIN] = SERIAL_BUFFER_LEN;
		tcflush(device, TCIFLUSH);
		tcsetattr(device, TCSANOW, &tio_new);
	}
}

void Serial::Serial_close(){
	tcsetattr(device, TCSANOW, &tio_old);
}

void Serial::Serial_write(unsigned char* msg, int len){
	int res = write(device, msg, len);
	if(res < 0){
		Serial_init(path.c_str(), baudrate, canonical_mode);
	}
}

char* Serial::Serial_read(){
	bzero(buf, SERIAL_BUFFER_LEN);
	int res = read(device, buf, SERIAL_BUFFER_LEN);
	return buf;
}

void Serial::Lock(){mu.lock();}
void Serial::Unlock(){mu.unlock();}
