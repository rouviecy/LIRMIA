#include "Cameras.h"

using namespace std;

Cameras::Cameras() : ComThread(){
	cam_detect1 = 0.;
	cam_detect2 = 0.;
}

Cameras::~Cameras(){}

void Cameras::On_start(){}

void Cameras::IO(){
	Link_output("cam_detect1", &cam_detect1);
	Link_output("cam_detect2", &cam_detect2);
}

void Cameras::Job(){
	Critical_receive();
	Critical_send();
}

cv::Mat Cameras::Get_img1(){return img1;}
cv::Mat Cameras::Get_img2(){return img2;}
