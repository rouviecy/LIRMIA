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

#include "../core/ComThread.h"
#include <opencv2/opencv.hpp>

class Cameras : public ComThread{

public:

	Cameras();
	~Cameras();

	cv::Mat Get_img1();
	cv::Mat Get_img2();

private:

	void On_start();
	void Job();
	void IO();

	float cam_detect1;
	float cam_detect2;

	cv::Mat img1, img2;

};

#endif
