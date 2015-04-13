#include "Serial.h"

using namespace std;

Serial::Serial(){}

void Serial::Serial_init(const char* path){
	device = open(path, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
	memset(&tio_new, 0, sizeof(tio_new));
	tcgetattr(device, &tio_old);
	cfsetispeed(&tio_new, B4800);
	cfsetospeed(&tio_new, B4800);
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

void Serial::Serial_close(){
	tcsetattr(device, TCSANOW, &tio_old);
}

void Serial::Serial_write(string msg){
	int msg_len = msg.length();
	unsigned char msg_char[msg_len];
	strcpy((char*) msg_char, msg.c_str());
	int res = write(device, msg_char, msg_len);
}

string Serial::Serial_read(){
	char buf[1024];
	int res = read(device, buf, 5); // TODO : \0 end
	return string(buf);
}
