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
#include "../interfaces/vision/Reco.h"
#include "../interfaces/Camera_server.h"
#include "../core/ComThread.h"
#include <opencv2/opencv.hpp>

class Cameras : public ComThread{

public:

	Cameras();
	~Cameras();

	cv::Mat Get_img1();
	cv::Mat Get_img2();

	Blobs* Get_blobs_obj();

private:

	#define CAMERA_PORT_1 4243
	#define CAMERA_PORT_2 4244
	#define CAMERA_PORT_3 4245
	#define CAMERA_PORT_4 4246

	cv::VideoCapture capture1, capture2;

	Blobs blobs;
	Camera_server camera_server;
	Reco reco;

	void On_start();
	void Job();
	void IO();

	float cam_detect_obj1, cam_detect_pipe1;
	float cam_detect_obj2, cam_detect_pipe2;

	float cam_detect1_horizontal, cam_detect1_vertical;
	float cam_detect2_horizontal, cam_detect2_vertical;
	float pipeline_angle_cam1, pipeline_distance_cam1;
	float pipeline_angle_cam2, pipeline_distance_cam2;

	cv::Mat img1, img2;

	std::vector <float> Find_biggest_blob(std::vector <cv::Point2i> blobs_center, std::vector <double> blobs_size, cv::Size img_size);

};

#endif
