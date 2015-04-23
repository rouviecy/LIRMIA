#include "Cameras.h"

using namespace std;

Cameras::Cameras() : ComThread(){
	cam_detect1 = -1.;
	cam_detect2 = -1.;
	cam_detect1_horizontal = 0.; cam_detect1_vertical = 0.;
	cam_detect2_horizontal = 0.; cam_detect2_vertical = 0.;
	#ifdef ENABLE_CAM1
		capture1 = cv::VideoCapture(0);
		#ifdef ENABLE_TCPCAM
			if(!tcp_server_cam1.Configure(4243)){return;}
			cout << "Waiting client for camera 1..." << endl;
			while(tcp_server_cam1.Get_nb_clients() == 0){
				usleep(1000000);
			}
			cout << "Client connected for camera 1" << endl;
		#endif
	#endif
	#ifdef ENABLE_CAM2
		capture2 = cv::VideoCapture(1);
		#ifdef ENABLE_TCPCAM
			if(!tcp_server_cam2.Configure(4244)){return;}
			cout << "Waiting client for camera 2..." << endl;
			while(tcp_server_cam2.Get_nb_clients() == 0){
				usleep(1000000);
			}
			cout << "Client connected for camera 2" << endl;
		#endif
	#endif
	
	
}

Cameras::~Cameras(){
	#ifdef ENABLE_TCPCAM
		#ifdef ENABLE_CAM1
			tcp_server_cam1.Close();
		#endif
		#ifdef ENABLE_CAM2
			tcp_server_cam2.Close();
		#endif
	#endif
}

void Cameras::On_start(){}

void Cameras::IO(){
	Link_output("cam_detect1", &cam_detect1);
	Link_output("cam_detect2", &cam_detect2);
	Link_output("cam_detect1_horizontal", &cam_detect1_horizontal); Link_output("cam_detect1_vertical", &cam_detect1_vertical);
	Link_output("cam_detect2_horizontal", &cam_detect2_horizontal); Link_output("cam_detect2_vertical", &cam_detect2_vertical);
}

void Cameras::Job(){
	#ifdef ENABLE_CAM1
		capture1 >> img1;
		blobs.Set_img(img1);
		blobs.Separer();
		blobs.Trouver_blobs();
		vector <float> blob_img1 = Find_biggest_blob(blobs.Get_mc(), blobs.Get_size(), blobs.Get_img_blobs().size());
		if(blob_img1.size() > 0){
			cam_detect1 = +1.;
			cam_detect1_horizontal = blob_img1[0];
			cam_detect1_vertical = blob_img1[1];
		}
		else{
			cam_detect1 = -1.;
		}
		#ifdef ENABLE_TCPCAM
			Send_tcp_img(img1, &tcp_server_cam1);
		#endif
	#endif

	#ifdef ENABLE_CAM2
		capture2 >> img2;
		blobs.Set_img(img2);
		blobs.Separer();
		blobs.Trouver_blobs();
		vector <float> blob_img2 = Find_biggest_blob(blobs.Get_mc(), blobs.Get_size(), blobs.Get_img_blobs().size());
		if(blob_img2.size() > 0){
			cam_detect2 = +1.;
			cam_detect2_horizontal = blob_img2[0];
			cam_detect2_vertical = blob_img2[1];
		}
		else{
			cam_detect2 = -1.;
		}
		#ifdef ENABLE_TCPCAM
			Send_tcp_img(img2, &tcp_server_cam2);
		#endif
	#endif

	Critical_send();
}

vector <float> Cameras::Find_biggest_blob(vector <cv::Point2i> blobs_center, vector <double> blobs_size, cv::Size img_size){
	vector <float> result;
	double max_size = 0;
	int index_biggest = 0;
	for(size_t i = 0; i < blobs_size.size(); i++){
		if(blobs_size[i] > max_size){
			max_size = blobs_size[i];
			index_biggest = i;
		}
	}
	if(max_size > 100){
		result.push_back((float) (2 * blobs_center[index_biggest].x) / img_size.width - 1.);
		result.push_back((float) (2 * blobs_center[index_biggest].y) / img_size.height - 1.);
	}
	return result;
}

void Cameras::Send_tcp_img(cv::Mat img, TCP_server* tcp){
	cv::Mat img_mini;
	cv::resize(img, img_mini, cv::Size(320, 240));
	img_mini = img_mini.reshape(0,1);
	int size_img = img_mini.total() * img_mini.elemSize();
	tcp->Direct_send(img_mini.data, size_img);
}

cv::Mat Cameras::Get_img1(){return img1;}
cv::Mat Cameras::Get_img2(){return img2;}
