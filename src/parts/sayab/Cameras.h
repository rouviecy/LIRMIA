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

#include "../../interfaces/vision/Blobs.h"
#include "../../interfaces/Camera_server.h"
#include "../../core/ComThread.h"
#include <opencv2/opencv.hpp>

class Cameras : public ComThread{

public:

	Cameras();
	~Cameras();

	Blobs* Get_blobs_obj();

private:

	#define CAMERA_PORT_1 5243
	#define CAMERA_PORT_2 5244
	#define CAMERA_PORT_3 5245
	#define CAMERA_PORT_4 5246

	cv::VideoCapture capture1, capture2;

	Blobs blobs;
	Camera_server camera_server;

	void On_start();
	void Job();
	void IO();

	bool enable_streaming;
	bool cam_detect_obj[2];
	float cam_detect_horizontal[2];
	float cam_detect_vertical[2];
	float cam_detect_size[2];

	cv::Mat img1, img2;

	std::vector <float> Find_biggest_blob(std::vector <cv::Point2i> blobs_center, std::vector <double> blobs_size, cv::Size img_size);

};

#endif
