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
#include "../../interfaces/vision/hsv_params.h"
#include "../../interfaces/Camera_server.h"
#include "../../core/ComThread.h"
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

	Blobs blobs, blobs_extra;
	Camera_server camera_server;
	hsv_params* hsv_extra;
	Reco reco;

	void On_start();
	void Job();
	void IO();

	bool enable_streaming;
	bool cam_detect_obj[2];		bool cam_detect_pipe[2];
	float cam_detect_horizontal[2];	float cam_detect_vertical[2];
	float cam_pipeline_angle[2];	float cam_pipeline_distance[2];
	float cam_size_obj[2];
	bool cam_detect_opi;
	float cam_opi_horizontal, cam_opi_vertical, cam_size_opi;

	cv::Mat img1, img2;

	void Find_blobs(cv::Mat* img, Blobs* blobs_finder, bool* out_detected, float* out_horizontal, float* out_vertical, float* out_size);

};

#endif
