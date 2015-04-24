/*
 * @(#)		Cameras.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Two cameras taking pictures
 */

#ifndef CAMERAS
#define CAMERAS

#include "../interfaces/vision/Blobs.h"
#include "../interfaces/vision/Gui.h"
#include "../interfaces/vision/Reco.h"
#include "../interfaces/TCP_server.h"
#include "../core/ComThread.h"
#include <opencv2/opencv.hpp>

class Cameras : public ComThread{

public:

	Cameras();
	~Cameras();

	cv::Mat Get_img1();
	cv::Mat Get_img2();

private:

	cv::VideoCapture capture1, capture2;

	Blobs blobs;
	Gui gui;
	Reco reco;
	TCP_server tcp_server_cam1, tcp_server_cam2;

	void On_start();
	void Job();
	void IO();

	float cam_detect1;
	float cam_detect2;

	float cam_detect1_horizontal, cam_detect1_vertical;
	float cam_detect2_horizontal, cam_detect2_vertical;

	cv::Mat img1, img2;

	std::vector <float> Find_biggest_blob(std::vector <cv::Point2i> blobs_center, std::vector <double> blobs_size, cv::Size img_size);
	void Send_tcp_img(cv::Mat img, TCP_server* tcp);

};

#endif
