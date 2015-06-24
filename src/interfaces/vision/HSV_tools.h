/*
 * @(#)		HSV_tools.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Select HSV boundaries and send it by TCP
 */

#ifndef HSV_TOOLS
#define HSV_TOOLS

#include <fstream>
#include <opencv2/opencv.hpp>
#include "../TCP_client.h"

class HSV_tools{

public: 
	void Create_HSV_params();
	void Create_HSV_trackbars();
	void Send_first_HSV_params();
	void Write_HSV_params();

	int H_min, S_min, V_min;
	int H_max, S_max, V_max;
	int nb_dilate, nb_erode;
	int seuil;
	TCP_client* tcp;
	char winname[128];
	char name_H_min[128], name_S_min[128], name_V_min[128];
	char name_H_max[128], name_S_max[128], name_V_max[128];
	char name_nb_dilate[128], name_nb_erode[128];
	char name_seuil[128];

private:
	void Verify_HSV_params();
	static void Send_HSV_param(void* object, std::string header, int* param);
	static void Callback_H_min	(int value, void* object);
	static void Callback_H_max	(int value, void* object);
	static void Callback_S_min	(int value, void* object);
	static void Callback_S_max	(int value, void* object);
	static void Callback_V_min	(int value, void* object);
	static void Callback_V_max	(int value, void* object);
	static void Callback_nb_dilate	(int value, void* object);
	static void Callback_nb_erode	(int value, void* object);
	static void Callback_seuil	(int value, void* object);

};

#endif
