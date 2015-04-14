#include "Mapping.h"

using namespace std;

Mapping::Mapping() : ComThread(){

}

Mapping::~Mapping(){}

void Mapping::On_start(){}

void Mapping::IO(){
	Link_input("x", &x);      Link_input("y", &y);      Link_input("z", &z);
	Link_input("thx", &thx);  Link_input("thy", &thy);  Link_input("thz", &thz);
	Link_input("obst_x", &x); Link_input("obst_y", &y);
}

void Mapping::Job(){
	Critical_receive();
}

void Mapping::Link_img(cv::Mat img1, cv::Mat img2){
	this->img1 = img1;
	this->img2 = img2;
}
