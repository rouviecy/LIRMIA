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

	bool enable_streaming;
	bool cam_detect_obj[2];		bool cam_detect_pipe[2];
	float cam_detect_horizontal[2];	float cam_detect_vertical[2];
	float cam_pipeline_angle[2];	float cam_pipeline_distance[2];
	float cam_size_obj[2];

	cv::Mat img1, img2;

	std::vector <float> Find_biggest_blob(std::vector <cv::Point2i> blobs_center, std::vector <double> blobs_size, cv::Size img_size);

};

#endif
