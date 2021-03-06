#include "Calibrate.h"

using namespace std;

// Constructeur
Calibrate::Calibrate(cv::Size dim_chess){
	this->dim_chess = dim_chess;
	nb_img_calib = 0;
	mat_cam = cv::Mat(cv::Size(3, 3), CV_32FC1);
	mat_cam.at<float>(0, 0) = 1; mat_cam.at<float>(1, 1) = 1;
	for(int i = 0; i < dim_chess.width * dim_chess.height; i++){
		obj.push_back(cv::Point3f(i / dim_chess.width, i % dim_chess.width, 0.0f));
	}
}

// Tenter d'ajouter un vecteur à la liste de calibration
int Calibrate::Ajouter_img_calib(cv::Mat img_input){
	std::vector <cv::Point2f> corners;
	bool trouve = findChessboardCorners(img_input, dim_chess, corners, CV_CALIB_CB_FAST_CHECK);
	if(!trouve){return nb_img_calib;}
	pts_image.push_back(corners);
	pts_objet.push_back(obj);
	nb_img_calib++;
	return nb_img_calib;
}

// Calibrer la caméra
void Calibrate::Calibrer(cv::Size taille){
	cv::calibrateCamera(pts_objet, pts_image, taille, mat_cam, dist_coeffs, rvecs, tvecs);
}

// Getters et Setters
cv::Mat Calibrate::Get_img(){
	cv::undistort(img, img_calib, mat_cam, dist_coeffs);
	return img_calib;
}
void Calibrate::Set_img(cv::Mat img){this->img = img;}
