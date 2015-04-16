#include "Cameras.h"

using namespace std;

Cameras::Cameras() : ComThread(){
	cam_detect1 = -1.;
	cam_detect2 = -1.;
	cam_detect1_horizontal = 0.; cam_detect1_vertical = 0.;
	cam_detect2_horizontal = 0.; cam_detect2_vertical = 0.;
	#ifdef ENABLE_CAM1
		capture1 = cv::VideoCapture(0);
	#endif
	#ifdef ENABLE_CAM2
		capture2 = cv::VideoCapture(1);
	#endif
}

Cameras::~Cameras(){

}

void Cameras::On_start(){}

void Cameras::IO(){
	Link_output("cam_detect1", &cam_detect1);
	Link_output("cam_detect2", &cam_detect2);
	Link_output("cam_detect1_horizontal", &cam_detect1_horizontal); Link_output("cam_detect1_vertical", &cam_detect1_vertical);
	Link_output("cam_detect2_horizontal", &cam_detect2_horizontal); Link_output("cam_detect2_vertical", &cam_detect2_vertical);
}

void Cameras::Job(){
	#ifdef ENABLE_CAM1
		capture1 >> img1;
		blobs.Set_img(img1);
		blobs.Separer();
		blobs.Trouver_blobs();
		vector <float> blob_img1 = Find_biggest_blob(blobs.Get_mc(), blobs.Get_size(), blobs.Get_img_blobs().size());
		if(blob_img1.size() > 0){
			cam_detect1 = +1.;
			cam_detect1_horizontal = blob_img1[0];
			cam_detect1_vertical = blob_img1[1];
		}
		else{
			cam_detect1 = -1.;
		}
	#endif

	#ifdef ENABLE_CAM2
		capture2 >> img2;
		blobs.Set_img(img2);
		blobs.Separer();
		blobs.Trouver_blobs();
		vector <float> blob_img2 = Find_biggest_blob(blobs.Get_mc(), blobs.Get_size(), blobs.Get_img_blobs().size());
		if(blob_img2.size() > 0){
			cam_detect2 = +1.;
			cam_detect2_horizontal = blob_img2[0];
			cam_detect2_vertical = blob_img2[1];
		}
		else{
			cam_detect2 = -1.;
		}
	#endif

	Critical_send();
}

vector <float> Cameras::Find_biggest_blob(vector <cv::Point2i> blobs_center, vector <double> blobs_size, cv::Size img_size){
	vector <float> result;
	double max_size = 0;
	int index_biggest = 0;
	for(size_t i = 0; i < blobs_size.size(); i++){
		if(blobs_size[i] > max_size){
			max_size = blobs_size[i];
			index_biggest = i;
		}
	}
	if(max_size > 100){
		result.push_back((float) (2 * blobs_center[index_biggest].x) / img_size.width - 1.);
		result.push_back((float) (2 * blobs_center[index_biggest].y) / img_size.height - 1.);
	}
	return result;
}

cv::Mat Cameras::Get_img1(){return img1;}
cv::Mat Cameras::Get_img2(){return img2;}
