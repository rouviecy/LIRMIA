/*
 * @(#)		Mapping.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Generate map
 */

#ifndef MAPPING
#define MAPPING

#include "../core/ComThread.h"
#include <opencv2/opencv.hpp>

class Mapping : public ComThread{

public:

	Mapping();
	~Mapping();

	void Link_img(cv::Mat img1, cv::Mat img2);

private:

	void On_start();
	void Job();
	void IO();
	
	float xyz[3];
	float thxyz[3];
	float obst_xy[2];

	cv::Mat img1, img2;

};

#endif
