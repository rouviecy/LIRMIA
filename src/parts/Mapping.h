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
	
	float x,   y,   z;
	float thx, thy, thz;
	float obst_x, obst_y;

	cv::Mat img1, img2;

};

#endif
