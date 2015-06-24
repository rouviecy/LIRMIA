#include "HSV_tools.h"

using namespace std;

void HSV_tools::Create_HSV_params(){
	ifstream in_file("./test/hsv_params.txt");
	if(in_file.is_open()){
		char line_char[256];
		in_file.getline(line_char, 256);
		in_file.getline(line_char, 256);
		string line_str = string(line_char);
		size_t next;
		vector <string> tokens;
		for(size_t current = 0; tokens.size() < 9; current = next + 1){
			next = line_str.find_first_of(",", current);
			tokens.push_back(line_str.substr(current, next - current));
		}
		H_min = stoi(tokens[0]);	S_min = stoi(tokens[2]);	V_min = stoi(tokens[4]);
		H_max = stoi(tokens[1]);	S_max = stoi(tokens[3]);	V_max = stoi(tokens[5]);
		nb_dilate = stoi(tokens[6]);	nb_erode = stoi(tokens[7]);	seuil = stoi(tokens[8]);
		in_file.close();
	}
	else{
		H_min = 80;	S_min = 100;	V_min = 100;
		H_max = 130;	S_max = 255;	V_max = 255;
		nb_dilate = 5;	nb_erode = 5;	seuil = 100;
	}
	strcpy(name_H_min, "H_min");		strcpy(name_H_max, "H_max");
	strcpy(name_S_min, "S_min");		strcpy(name_S_max, "S_max");
	strcpy(name_V_min, "V_min");		strcpy(name_V_max, "V_max");
	strcpy(name_nb_dilate, "nb_dilate");	strcpy(name_nb_erode, "nb_erode");
	strcpy(name_seuil, "seuil");		strcpy(winname, "HSV parameters");
}

void HSV_tools::Create_HSV_trackbars(){
	cv::namedWindow(winname, CV_WINDOW_AUTOSIZE);
	cv::moveWindow(winname, 0, 25);
	cv::createTrackbar(name_H_min,		winname,	&H_min,		360,	Callback_H_min,		this);
	cv::createTrackbar(name_H_max,		winname,	&H_max,		360,	Callback_H_max,		this);
	cv::createTrackbar(name_S_min,		winname,	&S_min,		255,	Callback_S_min,		this);
	cv::createTrackbar(name_S_max,		winname,	&S_max,		255,	Callback_S_max,		this);
	cv::createTrackbar(name_V_min,		winname,	&V_min,		255,	Callback_V_min,		this);
	cv::createTrackbar(name_V_max,		winname,	&V_max,		255,	Callback_V_max,		this);
	cv::createTrackbar(name_nb_dilate,	winname,	&nb_dilate,	20,	Callback_nb_dilate,	this);
	cv::createTrackbar(name_nb_erode,	winname,	&nb_erode,	20,	Callback_nb_erode,	this);
	cv::createTrackbar(name_seuil,		winname,	&seuil,		1000,	Callback_seuil,		this);
}

void HSV_tools::Verify_HSV_params(){
	if(H_min > H_max){cv::setTrackbarPos(name_H_max, winname, H_min);};
	if(S_min > S_max){cv::setTrackbarPos(name_S_max, winname, S_min);};
	if(V_min > V_max){cv::setTrackbarPos(name_V_max, winname, V_min);};
	cv::Mat img_color = cv::Mat::zeros(100, 300, CV_8UC3);
	cv::rectangle(img_color, cv::Point(0, 0), cv::Point(150, 100), cv::Scalar(H_min, S_min, V_min), CV_FILLED);
	cv::rectangle(img_color, cv::Point(150, 0), cv::Point(300, 100), cv::Scalar(H_max, S_max, V_max), CV_FILLED);
	cv::cvtColor(img_color, img_color, CV_HSV2RGB, 3);
	cv::imshow((char*) winname, img_color);
}

void HSV_tools::Send_HSV_param(void* object, string header, int* param){
	HSV_tools* hsv = (HSV_tools*) object;
	hsv->Verify_HSV_params();
	if(hsv->tcp != NULL){
		hsv->tcp->Send(header + to_string(*param));
	}
}

void HSV_tools::Send_first_HSV_params(){
	Callback_H_min(H_min, this);
	Callback_H_max(H_max, this);
	Callback_S_min(S_min, this);
	Callback_S_max(S_max, this);
	Callback_V_min(V_min, this);
	Callback_V_max(V_max, this);
	Callback_nb_dilate(nb_dilate, this);
	Callback_nb_erode(nb_erode, this);
	Callback_seuil(seuil, this);
}

void HSV_tools::Write_HSV_params(){
	ofstream out_file("./test/hsv_params.txt");
	string out_text = "H_min, H_max, S_min, S_max, V_min, V_max, nb_dilate, nb_erode, seuil,\n";
	int tab[] = {H_min, H_max, S_min, S_max, V_min, V_max, nb_dilate, nb_erode, seuil};
	for(int i = 0; i < 9; i++){out_text += to_string(tab[i]) + (i < 8 ? "," : ",\n");}
	out_file << out_text;
	out_file.close();
}

void HSV_tools::Callback_H_min		(int value, void* object){Send_HSV_param(object, "hsv_h0_", &(((HSV_tools*) object)->H_min));}
void HSV_tools::Callback_H_max		(int value, void* object){Send_HSV_param(object, "hsv_h1_", &(((HSV_tools*) object)->H_max));}
void HSV_tools::Callback_S_min		(int value, void* object){Send_HSV_param(object, "hsv_s0_", &(((HSV_tools*) object)->S_min));}
void HSV_tools::Callback_S_max		(int value, void* object){Send_HSV_param(object, "hsv_s1_", &(((HSV_tools*) object)->S_max));}
void HSV_tools::Callback_V_min		(int value, void* object){Send_HSV_param(object, "hsv_v0_", &(((HSV_tools*) object)->V_min));}
void HSV_tools::Callback_V_max		(int value, void* object){Send_HSV_param(object, "hsv_v1_", &(((HSV_tools*) object)->V_max));}
void HSV_tools::Callback_nb_dilate	(int value, void* object){Send_HSV_param(object, "hsv_m0_", &(((HSV_tools*) object)->nb_dilate));}
void HSV_tools::Callback_nb_erode	(int value, void* object){Send_HSV_param(object, "hsv_m1_", &(((HSV_tools*) object)->nb_erode));}
void HSV_tools::Callback_seuil		(int value, void* object){Send_HSV_param(object, "hsv_t0_", &(((HSV_tools*) object)->seuil));}
