#include "Cameras.h"

using namespace std;

Cameras::Cameras() : ComThread(){
	for(int i = 0; i < 2; i++){
		cam_detect_obj[i] = false;	cam_detect_pipe[i] = false;
		cam_detect_horizontal[i] = 0.;	cam_detect_vertical[i] = 0.;
		cam_pipeline_angle[i] = 0.;	cam_pipeline_distance[i] = 0.;
		cam_size_obj[i] = 0.;
	}
	cam_detect_opi = false;
	cam_opi_horizontal = 0.;
	cam_opi_vertical = 0.;
	hsv_extra = new hsv_params;
	hsv_extra->H_min = 113;
	hsv_extra->H_max = 137;
	hsv_extra->S_min = 161;
	hsv_extra->S_max = 255;
	hsv_extra->V_min = 108;
	hsv_extra->V_max = 255;
	hsv_extra->nb_dilate = 9;
	hsv_extra->nb_erode = 9;
	hsv_extra->seuil = 1000;
	blobs_extra.Definir_limites_separation(hsv_extra);
	delete hsv_extra;

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

	#ifdef ENABLE_RECORDCAM
		#if defined(ENABLE_CAM1) and defined(ENABLE_CAM2)
			camera_server.Enable_record(2);
		#elif defined(ENABLE_CAM1)
			camera_server.Enable_record(1);
		#elif defined(ENABLE_CAM2)
			camera_server.Enable_record(1);
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
	Link_output("cam_detect_opi",		COMBOOL,	1, &cam_detect_opi);
	Link_output("cam_opi_horizontal",	COMFLOAT,	1, &cam_opi_horizontal);
	Link_output("cam_opi_vertical",		COMFLOAT,	1, &cam_opi_vertical);
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
		if(enable_streaming){
			#ifdef ENABLE_RECORDCAM
				camera_server.Record_img(img1, 0);
			#endif
			#ifdef ENABLE_TCPCAM
				camera_server.Send_tcp_img(img1, CAMERA_PORT_1);
				camera_server.Send_tcp_img(img_pipeline1, CAMERA_PORT_3);
			#endif
		}
	#endif

	#ifdef ENABLE_CAM2
		capture2 >> img2;

		// Find blobs (with pipeline color)
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

		// Find pipeline
		reco.Set_img(blobs.Get_img_blobs());
		cv::Mat img_pipeline2 = reco.Trouver_ligne_principale(&(cam_detect_pipe[1]), &(cam_pipeline_angle[1]), &(cam_pipeline_distance[1]));

		// Find blobs (with another color)
		blobs_extra.Set_img(img2);
		blobs_extra.Separer();
		blobs_extra.Trouver_blobs();
		vector <float> blob_extra_img = Find_biggest_blob(blobs_extra.Get_mc(), blobs_extra.Get_size(), blobs_extra.Get_img_blobs().size());
		cam_detect_opi = (blob_extra_img.size() > 0);
		if(cam_detect_opi){
cout << "Détection d'une OPI" << endl;
			cam_opi_horizontal = blob_extra_img[0];
			cam_opi_vertical = blob_extra_img[1];
			cam_size_opi = blob_extra_img[2];
		}
		if(enable_streaming){
			#ifdef ENABLE_RECORDCAM
				camera_server.Record_img(img2, 1);
			#endif
			#ifdef ENABLE_TCPCAM
				camera_server.Send_tcp_img(img2, CAMERA_PORT_2);
				camera_server.Send_tcp_img(img_pipeline2, CAMERA_PORT_4);
			#endif
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
		result.push_back((float) (max_size / (img_size.width * img_size.height)));
	}
	return result;
}

cv::Mat Cameras::Get_img1(){return img1;}
cv::Mat Cameras::Get_img2(){return img2;}

Blobs* (Cameras::Get_blobs_obj)(){return &blobs;}
