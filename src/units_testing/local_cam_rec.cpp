#include "../interfaces/Camera_server.h"

using namespace std;

int main(){
	Camera_server recorder;
	int nb_cams = 0;
	cv::VideoCapture capture1, capture2;
	cv::Mat img1, img2;

	#ifdef ENABLE_CAM1
		capture1.open(0);
		nb_cams++;
	#endif
	#ifdef ENABLE_CAM2
		capture2.open(1);
		nb_cams++;
	#endif

	recorder.Enable_record(nb_cams);

	while(true){
		#ifdef ENABLE_CAM1
			capture1 >> img1;
			recorder.Record_img(img1, 0);
		#endif
		#ifdef ENABLE_CAM2
			capture2 >> img2;
			recorder.Record_img(img2, 1);
		#endif
		cv::waitKey(40);
	}

	return 0;
}
