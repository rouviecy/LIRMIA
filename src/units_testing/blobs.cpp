#include "../parts/lirmia/Cameras.h"

using namespace std;


int main(int argc, char* argv[]){
	Blobs blobs;
	HSV_tools hsv;
	hsv.Create_HSV_params();
	hsv.tcp = NULL;
	hsv.Create_HSV_trackbars();
	hsv.Send_first_HSV_params();

	cv::Mat img_input = cv::imread(argv[1]);
	cv::Mat img_blobs;
	cv::imshow("Raw image", img_input);
	bool detected_obj,	detected_pipe;
	float horizontal_obj,	vertical_obj;
	float size_obj;

	for(char key = 'a'; key != 'q'; key = cv::waitKey(40)){
		blobs.Definir_limites_separation(&hsv);
		Cameras::Find_blobs(&img_input, &blobs, &detected_obj, &horizontal_obj, &vertical_obj, &size_obj);
		blobs.Get_img_blobs().copyTo(img_blobs);
		std::vector <cv::Point2i> list_mc = blobs.Get_mc();
		std::vector <cv::Rect> list_rect = blobs.Get_rect();
		for(int i = 0; i < list_mc.size(); i++){
			cv::circle(img_blobs, list_mc[i], 1, cv::Scalar(0, 0, 255), 2);
			cv::rectangle(img_blobs, list_rect[i], cv::Scalar(0, 0, 255), 3);
		}
		cv::imshow("Blobs", img_blobs);
	}

	cv::imwrite("img_blobs.png", img_blobs);
}
