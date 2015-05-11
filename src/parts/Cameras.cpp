#include "Cameras.h"

using namespace std;

Cameras::Cameras() : ComThread(){
	for(int i = 0; i < 2; i++){
		cam_detect_obj[i] = false;	cam_detect_pipe[i] = false;
		cam_detect_horizontal[i] = 0.;	cam_detect_vertical[i] = 0.;
		cam_pipeline_angle[i] = 0.;	cam_pipeline_distance[i] = 0.;
		cam_size_obj[i] = 0.;
	}
	
	#ifdef ENABLE_CAM1
		capture1 = cv::VideoCapture(0);
		#ifdef ENABLE_TCPCAM
			camera_server.Add_flux(CAMERA_PORT_1);
			camera_server.Add_flux(CAMERA_PORT_3);
		#endif
	#endif
	#ifdef ENABLE_CAM2
		capture2 = cv::VideoCapture(1);
		#ifdef ENABLE_TCPCAM
			camera_server.Add_flux(CAMERA_PORT_2);
			camera_server.Add_flux(CAMERA_PORT_4);
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
	
	Link_output("cam_detect_obj",		COMBOOL,	2, cam_detect_obj);
	Link_output("cam_detect_pipe",		COMBOOL,	2, cam_detect_pipe);
	Link_output("cam_detect_horizontal",	COMFLOAT,	2, cam_detect_horizontal);
	Link_output("cam_detect_vertical",	COMFLOAT,	2, cam_detect_vertical);
	Link_output("cam_pipeline_angle",	COMFLOAT,	2, cam_pipeline_angle);
	Link_output("cam_pipeline_distance",	COMFLOAT,	2, cam_pipeline_distance);
	Link_output("cam_size_obj",		COMFLOAT,	2, cam_size_obj);
}

void Cameras::Job(){
	Critical_receive();

	#ifdef ENABLE_CAM1
		capture1 >> img1;
		blobs.Set_img(img1);
		blobs.Separer();
		blobs.Trouver_blobs();
		vector <float> blob_img1 = Find_biggest_blob(blobs.Get_mc(), blobs.Get_size(), blobs.Get_img_blobs().size());
		cam_detect_obj[0] = (blob_img1.size() > 0);
		if(cam_detect_obj[0]){
			cam_detect_horizontal[0] = blob_img1[0];
			cam_detect_vertical[0] = blob_img1[1];
			cam_size_obj[0] = blob_img1[2];
		}
		reco.Set_img(blobs.Get_img_blobs());
		cv::Mat img_pipeline1 = reco.Trouver_ligne_principale(&(cam_detect_pipe[0]), &(cam_pipeline_angle[0]), &(cam_pipeline_distance[0]));
		#ifdef ENABLE_TCPCAM
			if(enable_streaming){
				camera_server.Send_tcp_img(img1, CAMERA_PORT_1);
				camera_server.Send_tcp_img(img_pipeline1, CAMERA_PORT_3);
			}
		#endif
	#endif

	#ifdef ENABLE_CAM2
		capture2 >> img2;
		blobs.Set_img(img2);
		blobs.Separer();
		blobs.Trouver_blobs();
		vector <float> blob_img2 = Find_biggest_blob(blobs.Get_mc(), blobs.Get_size(), blobs.Get_img_blobs().size());
		cam_detect_obj[1] = (blob_img2.size() > 0);
		if(cam_detect_obj[1]){
			cam_detect_horizontal[1] = blob_img2[0];
			cam_detect_vertical[1] = blob_img2[1];
			cam_size_obj[1] = blob_img2[2];
		}
		reco.Set_img(blobs.Get_img_blobs());
		cv::Mat img_pipeline2 = reco.Trouver_ligne_principale(&(cam_detect_pipe[1]), &(cam_pipeline_angle[1]), &(cam_pipeline_distance[1]));
		#ifdef ENABLE_TCPCAM
			if(enable_streaming){
				camera_server.Send_tcp_img(img2, CAMERA_PORT_2);
				camera_server.Send_tcp_img(img_pipeline2, CAMERA_PORT_4);
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

cv::Mat Cameras::Get_img1(){return img1;}
cv::Mat Cameras::Get_img2(){return img2;}

Blobs* (Cameras::Get_blobs_obj)(){return &blobs;}
