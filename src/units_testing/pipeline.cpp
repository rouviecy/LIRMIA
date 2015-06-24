#include "../parts/lirmia/Cameras.h"

using namespace std;


int main(int argc, char* argv[]){
	Blobs blobs;
	Reco reco;
	HSV_tools hsv;
	hsv.Create_HSV_params();
	hsv.tcp = NULL;
	hsv.Create_HSV_trackbars();

	cv::Mat img_input = cv::imread(argv[1]);
	bool detected_obj,	detected_pipe;
	float horizontal_obj,	angle_pipe;
	float vertical_obj,	distance_pipe;
	float size_obj;
	
	Cameras::Find_blobs(&img_input, &blobs, &detected_obj, &horizontal_obj, &vertical_obj, &size_obj);
	reco.Set_img(blobs.Get_img_blobs());
	cv::Mat img_pipeline = reco.Trouver_ligne_principale(&detected_pipe, &angle_pipe, &distance_pipe);
	cv::imwrite("img_pipeline.png", img_pipeline);
}
