/*
 * @(#)		Camera_client.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Receive camera video with TCP client
 */

#ifndef CAMERA_CLIENT
#define CAMERA_CLIENT

#include "./TCP_client.h"
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <vector>
#include <thread>
#include <mutex>

typedef struct{
	std::thread thr;
	std::mutex mu;
	char key;
	cv::Mat img;
	char* host;
	int port;
	std::string window_name;
}thread_cam_t;

class Camera_client{

public:

	void Subscribe_cam(std::string window_name, char* host, int port, int window_pos_x, int window_pos_y);
	void Show_video();

private:

	std::vector <thread_cam_t*> threads_cam;

	static void Open_cam(thread_cam_t* thread_cam);

};

#endif
