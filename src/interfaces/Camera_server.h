/*
 * @(#)		Camera_server.h
 * @version	2.0
 * @autor	C. Rouvière
 */

/**
 * Send camera video with TCP server and record it on hard-drive
 * 	- First, call Add_flux(int port) to create a new TCP server (one per video flow)
 * 	- If you also want to record on hard-drive, call once Enable_record(int nb_cams)
 * 	- Call Send_tcp_img(cv::Mat img, int port) to send an image (which will be encoded)
 * 	- Call Record_img(cv::Mat img, int cam_index) to send an image to a buffer (which is written to hard-drive when full)
 * 	- Call Direct_record_img(cv::Mat img, std::string id) to write an image to hard-drive without buffer
 */

#ifndef CAMERA_SERVER
#define CAMERA_SERVER

#include "../interfaces/TCP_server.h"
#include <opencv2/opencv.hpp>
#include <map>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <fstream>

#define CAM_REC_BUFFER_SIZE 25

class Camera_server{

public:

	Camera_server();
	~Camera_server();

	bool Add_flux(int port);
	void Clear_all_flux();

	void Enable_record(int nb_cams);
	void Disable_record();

	bool Send_tcp_img(cv::Mat img, int port);
	void Record_img(cv::Mat img, int cam_index);
	void Direct_record_img(cv::Mat img, std::string id);

private:

	std::map <int, TCP_server*> tcp_servers;

	int nb_cams;
	bool enabled_record;
	std::string path_record;
	std::vector <int> cycle_index;
	std::vector <int> cycle_position;
	std::vector <int> encode_params;
	std::vector <cv::Mat*> img_buffers;

	std::string Img_path_record(int cam_index, int img_index);

};

#endif
