/*
 * @(#)		Camera_server.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Send camera video with TCP server
 */

#ifndef CAMERA_SERVER
#define CAMERA_SERVER

#include "../interfaces/TCP_server.h"
#include <opencv2/opencv.hpp>
#include <map>

class Camera_server{

public:

	Camera_server();
	~Camera_server();

	bool Add_flux(int port);
	void Clear_all_flux();

	bool Send_tcp_img(cv::Mat img, int port);

private:

	std::map <int, TCP_server*> tcp_servers;

};

#endif
