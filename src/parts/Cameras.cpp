#include "Cameras.h"

using namespace std;

Cameras::Cameras() : ComThread(){
	cam_detect_obj1 = -1.;		cam_detect_pipe1 = -1.;
	cam_detect_obj2 = -1.;		cam_detect_pipe2 = -1.;
	cam_detect1_horizontal = 0.;	cam_detect1_vertical = 0.;
	cam_detect2_horizontal = 0.;	cam_detect2_vertical = 0.;
	pipeline_angle_cam1 = 0.;	pipeline_distance_cam1 = 0.;
	pipeline_angle_cam2 = 0.;	pipeline_distance_cam2 = 0.;
	
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
	Link_output("cam_detect_obj1", &cam_detect_obj1);		Link_output("cam_detect_pipe1", &cam_detect_pipe1);
	Link_output("cam_detect_obj2", &cam_detect_obj2);		Link_output("cam_detect_pipe2", &cam_detect_pipe2);
	Link_output("cam_detect1_horizontal", &cam_detect1_horizontal);	Link_output("cam_detect1_vertical", &cam_detect1_vertical);
	Link_output("cam_detect2_horizontal", &cam_detect2_horizontal);	Link_output("cam_detect2_vertical", &cam_detect2_vertical);
	Link_output("pipeline_angle_cam1", &pipeline_angle_cam1);	Link_output("pipeline_distance_cam1", &pipeline_distance_cam1);
	Link_output("pipeline_angle_cam2", &pipeline_angle_cam2);	Link_output("pipeline_distance_cam2", &pipeline_distance_cam2);
}

void Cameras::Job(){
	#ifdef ENABLE_CAM1
		capture1 >> img1;
		blobs.Set_img(img1);
		blobs.Separer();
		blobs.Trouver_blobs();
		vector <float> blob_img1 = Find_biggest_blob(blobs.Get_mc(), blobs.Get_size(), blobs.Get_img_blobs().size());
		if(blob_img1.size() > 0){
			cam_detect_obj1 = +1.;
			cam_detect1_horizontal = blob_img1[0];
			cam_detect1_vertical = blob_img1[1];
		}
		else{
			cam_detect_obj1 = -1.;
		}
		bool pipeline_detected_cam1;
		reco.Set_img(blobs.Get_img_blobs());
		cv::Mat img_pipeline1 = reco.Trouver_ligne_principale(&pipeline_detected_cam1, &pipeline_angle_cam1, &pipeline_distance_cam1);
		cam_detect_pipe1 = pipeline_detected_cam1 ? +1.0 : -1.0;
		#ifdef ENABLE_TCPCAM
			camera_server.Send_tcp_img(img1, CAMERA_PORT_1);
			camera_server.Send_tcp_img(img_pipeline1, CAMERA_PORT_3);
		#endif
	#endif

	#ifdef ENABLE_CAM2
		capture2 >> img2;
		blobs.Set_img(img2);
		blobs.Separer();
		blobs.Trouver_blobs();
		vector <float> blob_img2 = Find_biggest_blob(blobs.Get_mc(), blobs.Get_size(), blobs.Get_img_blobs().size());
		if(blob_img2.size() > 0){
			cam_detect_obj2 = +1.;
			cam_detect2_horizontal = blob_img2[0];
			cam_detect2_vertical = blob_img2[1];
		}
		else{
			cam_detect_obj2 = -1.;
		}
		bool pipeline_detected_cam2;
		reco.Set_img(blobs.Get_img_blobs());
		cv::Mat img_pipeline2 = reco.Trouver_ligne_principale(&pipeline_detected_cam2, &pipeline_angle_cam2, &pipeline_distance_cam2);
		cam_detect_pipe2 = pipeline_detected_cam2 ? +1.0 : -1.0;
		#ifdef ENABLE_TCPCAM
			camera_server.Send_tcp_img(img2, CAMERA_PORT_2);
			camera_server.Send_tcp_img(img_pipeline2, CAMERA_PORT_4);
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
	}
	return result;
}

cv::Mat Cameras::Get_img1(){return img1;}
cv::Mat Cameras::Get_img2(){return img2;}

Blobs* (Cameras::Get_blobs_obj)(){return &blobs;}
