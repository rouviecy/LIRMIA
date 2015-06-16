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
#include "../../interfaces/vision/Reco.h"
#include "../../interfaces/Camera_server.h"
#include "../../core/ComThread.h"
#include <opencv2/opencv.hpp>

class Cameras : public ComThread{

public:

	Cameras();
	~Cameras();

	Blobs* Get_blobs_obj();

private:

	#define CAMERA_PORT_1 6243
	#define CAMERA_PORT_3 6245

	cv::VideoCapture capture;

	Blobs blobs;
	Camera_server camera_server;
	Reco reco;

	void On_start();
	void Job();
	void IO();

	bool enable_streaming;
	bool cam_detect_obj;
	float cam_detect_horizontal;
	float cam_detect_vertical;
	float cam_size_obj;

	cv::Mat img;

	std::vector <float> Find_biggest_blob(std::vector <cv::Point2i> blobs_center, std::vector <double> blobs_size, cv::Size img_size);

};

#endif
