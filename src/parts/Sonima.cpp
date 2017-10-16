#include "Sonima.h"

using namespace std;

Sonima::Sonima() : ComThread(){
	int w=200;
	int w2=w/2;
	float PI=3.141592;
	int cont=0;
	#ifdef ENABLE_TCPCAM
			sonar_server.Add_flux( 6247);
			sonar_server.Add_flux( 6248);
	#endif
	//cam_detect_obj = false;
	//cam_detect_horizontal = 0.;
	//cam_detect_vertical = 0.;
	//cam_size_obj= 0.;
	/*
	#ifdef ENABLE_CAM1
		capture = cv::VideoCapture(0);
		capture.set(CV_CAP_PROP_FRAME_WIDTH, 400);
		capture.set(CV_CAP_PROP_FRAME_HEIGHT, 300);
		#ifdef ENABLE_TCPCAM
			camera_server.Add_flux(CAMERA_PORT_1);
			camera_server.Add_flux(CAMERA_PORT_3);
		#endif
	#endif*/

}

Sonima::~Sonima(){
	#ifdef ENABLE_TCPCAM
		sonar_server.Clear_all_flux();
	#endif
}

void Sonima::On_start(){	
	polar = cv::Mat::zeros( 200, 200, CV_8UC1 );
	iplimgp = polar;	
	image = cv::Mat::zeros( 360, 200, CV_8UC1 );
	iplimgi = image;
	cont=0;
}

void Sonima::IO(){
	Link_input("sData", 	COMFLOAT,  1,&sData);	
	Link_input("fready", 	COMFLOAT,  1,&fready);
	/*Link_output("cam_detect_obj",		COMBOOL,	1, &cam_detect_obj);
	Link_output("cam_detect_horizontal",	COMFLOAT,	1, &cam_detect_horizontal);
	Link_output("cam_detect_vertical",	COMFLOAT,	1, &cam_detect_vertical);
	Link_output("cam_size_obj",		COMFLOAT,	1, &cam_size_obj);*/
}

void Sonima::Job(){
	Critical_receive();
	//cout<<+sData[40]<<" | "<<sData[41]<<" | "<<sData[243]<<endl;
	if(fready==0){
		graa=sData[40];  
		grab=sData[41]*256;
  		grado=(graa+grab)*0.05625;//6400 puntos, 360 grados
  		grado2=grado;
		cont++;
		for (int i=0;i<245;i++){		
			if(sData[i]<255 && i>44 && i<244){	
				image.at<uchar>(grado2,i-44)=int(sData[i]);	
			}				
		}
	
	}
//	sonar_server.Send_tcp_img(image,  6247);
	cv::Point2f center( (float)polar.cols / 2, (float)polar.rows / 2 );
	cvLinearPolar(&iplimgi, &iplimgp, center, 100,CV_WARP_INVERSE_MAP );
	cv::Mat image2, image3, polar2, polar3;
	cv::threshold(image,image2,100,255,3);
	cv::GaussianBlur(image2, image3, cv::Size(5,5),0,0);
	sonar_server.Send_tcp_img(image3,  6247);
	cv::threshold(polar,polar2,100,255,3);
	cv::GaussianBlur(polar2, polar3, cv::Size(5,5),0,0);
	sonar_server.Send_tcp_img(polar3,  6248);
	//usleep(500000);
	/*#ifdef ENABLE_CAM1
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
		#ifdef ENABLE_TCPCAM
			if(enable_streaming){
				camera_server.Send_tcp_img(img, CAMERA_PORT_1);
				camera_server.Send_tcp_img(blobs.Get_img_blobs(), CAMERA_PORT_3);
			}
		#endif
	#endif*/

	Critical_send();

}

/*vector <float> Cameras::Find_biggest_blob(vector <cv::Point2i> blobs_center, vector <double> blobs_size, cv::Size img_size){
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

Blobs* (Cameras::Get_blobs_obj)(){return &blobs;}*/
