/*
 * @(#)		Sonima.h
 * @version	1.0
 * @autor	L. Revueltas
 */

/**
 * Two cameras taking pictures
 */

#ifndef SONIMA
#define SONIMA

//#include "../../interfaces/vision/Blobs.h"
//#include "../../interfaces/vision/Reco.h"
//#include "../../interfaces/Camera_server.h"
#include "../core/ComThread.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv/cv.hpp>
#include "../interfaces/Camera_server.h"
#include <iostream>
#include <cmath>

class Sonima : public ComThread{

public:

	Sonima();
	~Sonima();

	//Blobs* Get_blobs_obj();

private:

	//#define CAMERA_PORT_1 6243
	//#define CAMERA_PORT_3 6245

	//cv::VideoCapture capture;

	//Blobs blobs;
	//Camera_server camera_server;
	//Reco reco;
	float* sData;
	int fready;
	//Para hacer el mapa de datos
	int w;
	int w2;
	float PI;
	signed int rhos[200][200],thes[200][200];
  	signed int c=0,f=0,d=0,alfa=0;
  	float alfa2,c2,f2;

	//Obtener el angulo
	float graa,grab,grado;
	int grado2;

	cv::Mat polar;
	cv::Mat image;
	IplImage iplimgp;
	IplImage iplimgi;

	Camera_server sonar_server;

	void On_start();
	void Job();
	void IO();
	int cont;

	//bool enable_streaming;
	//bool cam_detect_obj;
	//float cam_detect_horizontal;
	//float cam_detect_vertical;
	//float cam_size_obj;

	cv::Mat img;

	//std::vector <float> Find_biggest_blob(std::vector <cv::Point2i> blobs_center, std::vector <double> blobs_size, cv::Size img_size);

};

#endif
