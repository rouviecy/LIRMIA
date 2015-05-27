#include <opencv2/opencv.hpp>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <fstream>

#define BUFFER_SIZE 25

using namespace std;

cv::Mat Cam_to_jpeg(cv::VideoCapture* camera, vector <int> *params){
	cv::Mat img;
	std::vector <unsigned char> img_encoded;
	*camera >> img;
	cv::imencode(".jpg", img, img_encoded, *params);
	return cv::imdecode(img_encoded, CV_LOAD_IMAGE_COLOR);
}

string Generate_folder(){
	time_t t_now = time(0);
	struct tm* now = localtime(&t_now);
	stringstream path_ss;
	path_ss	<< "./test/"
		<< (now->tm_year + 1900) << "-"
		<< setfill('0') << setw(2) << (now->tm_mon + 1) << "-"
		<< setfill('0') << setw(2) << now->tm_mday << "--"
		<< setfill('0') << setw(2) << now->tm_hour << ":"
		<< setfill('0') << setw(2) << now->tm_min << ":"
		<< setfill('0') << setw(2) << now->tm_sec << "--Video/";
	string path = path_ss.str();
	string mkdir_command = "mkdir \"" + path + "\"";
	system(mkdir_command.c_str());
	return path;
}

string Img_path(string path, int cam_index, int img_index){
	stringstream img_path_ss;
	img_path_ss	<< path
			<< "CAM"
			<< cam_index
			<< "_"
			<< setfill('0') << setw(6) << img_index
			<< ".jpg";
	return img_path_ss.str();
}

int main(){
	cv::VideoCapture capture1, capture2;
	cv::Mat buffer1[BUFFER_SIZE];
	cv::Mat buffer2[BUFFER_SIZE];
	vector <int> encode_params;
	encode_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	encode_params.push_back(95);
	string path = Generate_folder();

	#ifdef ENABLE_CAM1
		capture1.open(0);
	#endif
	#ifdef ENABLE_CAM2
		capture2.open(1);
	#endif

	cv::waitKey(1000);
	int cycles = -1;
	for(int i = 0; true; i++){
		if(i == BUFFER_SIZE){
			i = 0;
			cycles++;
			for(int j = 0; j < BUFFER_SIZE; j++){
				int index = cycles * BUFFER_SIZE + j;
				#ifdef ENABLE_CAM1
					cv::imwrite(Img_path(path, 1, index), buffer1[j]);
				#endif
				#ifdef ENABLE_CAM2
					cv::imwrite(Img_path(path, 2, index), buffer2[j]);
				#endif
			}
		}
		#ifdef ENABLE_CAM1
			buffer1[i] = Cam_to_jpeg(&capture1, &encode_params);
		#endif
		#ifdef ENABLE_CAM2
			buffer2[i] = Cam_to_jpeg(&capture2, &encode_params);
		#endif
		cv::waitKey(40);
	}

	return 0;
}
