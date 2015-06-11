/*
 * @(#)		Camera_client.h
 * @version	2.0
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

	void Subscribe_cam(			// Subscribe (call once) to a video flow created by Camera_server
		std::string window_name,		// Name of the window where to show the video
		char* host,				// Camera_server IP
		int port,				// Camera_server port
		int window_pos_x,			// X position of the window
		int window_pos_y);			// Y position of the window
	void Show_video();			// Update all video after each call

private:

	std::vector <thread_cam_t*> threads_cam;

	static void Open_cam(thread_cam_t* thread_cam);

};

#endif
