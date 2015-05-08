#include "Mapping.h"

using namespace std;

Mapping::Mapping() : ComThread(){

}

Mapping::~Mapping(){}

void Mapping::On_start(){}

void Mapping::IO(){
	Link_input("xyz",	COMFLOAT, 3, xyz);
	Link_input("thxyz",	COMFLOAT, 3, thxyz);
	Link_input("obst_xy",	COMFLOAT, 2, obst_xy);
}

void Mapping::Job(){
	Critical_receive();
}

void Mapping::Link_img(cv::Mat img1, cv::Mat img2){
	this->img1 = img1;
	this->img2 = img2;
}
