#include "Cameras.h"

using namespace std;

Cameras::Cameras() : ComThread(){
	cam_detect_obj = false;		cam_detect_pipe = false;
	cam_detect_horizontal = 0.;	cam_detect_vertical = 0.;
	cam_pipeline_angle = 0.;	cam_pipeline_distance = 0.;
	cam_size_obj= 0.;

	#ifdef ENABLE_CAM1
		capture = cv::VideoCapture(0);
		capture.set(CV_CAP_PROP_FRAME_WIDTH, 400);
		capture.set(CV_CAP_PROP_FRAME_HEIGHT, 300);
		#ifdef ENABLE_TCPCAM
			camera_server.Add_flux(CAMERA_PORT_1);
			camera_server.Add_flux(CAMERA_PORT_3);
		#endif
	#endif

}

Cameras::~Cameras(){
	#ifdef ENABLE_TCPCAM
		camera_server.Clear_all_flux();
	#endif
}

void Cameras::On_start(){}

void Cameras::IO(){
	Link_input("enable_streaming",		COMBOOL,	1, &enable_streaming);

	Link_output("cam_detect_obj",		COMBOOL,	1, &cam_detect_obj);
	Link_output("cam_detect_pipe",		COMBOOL,	1, &cam_detect_pipe);
	Link_output("cam_detect_horizontal",	COMFLOAT,	1, &cam_detect_horizontal);
	Link_output("cam_detect_vertical",	COMFLOAT,	1, &cam_detect_vertical);
	Link_output("cam_pipeline_angle",	COMFLOAT,	1, &cam_pipeline_angle);
	Link_output("cam_pipeline_distance",	COMFLOAT,	1, &cam_pipeline_distance);
	Link_output("cam_size_obj",		COMFLOAT,	1, &cam_size_obj);
}

void Cameras::Job(){
	Critical_receive();

	#ifdef ENABLE_CAM1
		capture >> img;
		cv::flip(img, img, -1);
		blobs.Set_img(img);
		blobs.Separer();
		blobs.Trouver_blobs();
		vector <float> blob_img = Find_biggest_blob(blobs.Get_mc(), blobs.Get_size(), blobs.Get_img_blobs().size());
		cam_detect_obj = (blob_img.size() > 0);
		if(cam_detect_obj){
			cam_detect_horizontal = blob_img[0];
			cam_detect_vertical = blob_img[1];
			cam_size_obj = blob_img[2];
		}
		reco.Set_img(blobs.Get_img_blobs());
		cv::Mat img_pipeline = reco.Trouver_ligne_principale(&cam_detect_pipe, &cam_pipeline_angle, &cam_pipeline_distance);
		#ifdef ENABLE_TCPCAM
			if(enable_streaming){
				camera_server.Send_tcp_img(img, CAMERA_PORT_1);
				camera_server.Send_tcp_img(img_pipeline, CAMERA_PORT_3);
			}
		#endif
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
		result.push_back((float) (max_size / (img_size.width * img_size.height)));
	}
	return result;
}

Blobs* (Cameras::Get_blobs_obj)(){return &blobs;}
