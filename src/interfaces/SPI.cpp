#include "SPI.h"

using namespace std;

SPI::SPI(){}

void SPI::SPI_init(const char* path, int speed, int mode, bool little_endian, int bits_per_word){
	this->path = string(path);
	this->mode = mode;
	this->little_endian = little_endian;
	spi_transfer.speed_hz = speed;
	spi_transfer.delay_usecs = 0;
	spi_transfer.len = bits_per_word;
	device = open(path, O_RDWR);
        if(device < 0){
		cout << "[Error] Unable to open SPI port to " + this->path << endl;
		return;
	}
	ioctl(device, SPI_IOC_WR_MODE, &(this->mode));
	ioctl(device, SPI_IOC_RD_MODE, &(this->mode));
	ioctl(device, SPI_IOC_WR_BITS_PER_WORD, &(spi_transfer.len));
	ioctl(device, SPI_IOC_RD_BITS_PER_WORD, &(spi_transfer.len));
	ioctl(device, SPI_IOC_WR_LSB_FIRST, &(this->little_endian));
	ioctl(device, SPI_IOC_RD_LSB_FIRST, &(this->little_endian));
	ioctl(device, SPI_IOC_WR_MAX_SPEED_HZ, &(spi_transfer.speed_hz));
	ioctl(device, SPI_IOC_RD_MAX_SPEED_HZ, &(spi_transfer.speed_hz));
}

void SPI::SPI_close(){
	close(device);
}

void SPI::SPI_duplex(unsigned char* msg_in, unsigned char* msg_out, int len_in, int len_out){
	spi_transfer.rx_nbits = len_in;
	spi_transfer.tx_nbits = len_out;
	spi_transfer.rx_buf = (unsigned long) msg_in;
	spi_transfer.tx_buf = (unsigned long) msg_out;
	cout << ioctl(device, SPI_IOC_MESSAGE(1), &spi_transfer) << endl;
}

void SPI::Lock(){mu.lock();}
void SPI::Unlock(){mu.unlock();}
