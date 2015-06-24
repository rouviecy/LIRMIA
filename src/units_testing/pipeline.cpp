#include "../parts/lirmia/Cameras.h"

using namespace std;


int main(int argc, char* argv[]){
	Blobs blobs;
	Reco reco;
	HSV_tools hsv;
	hsv.Create_HSV_params();
	hsv.tcp = NULL;
	hsv.Create_HSV_trackbars();
	hsv.Send_first_HSV_params();

	cv::Mat img_input = cv::imread(argv[1]);
	cv::imshow("Raw image", img_input);
	bool detected_obj,	detected_pipe;
	float horizontal_obj,	angle_pipe;
	float vertical_obj,	distance_pipe;
	float size_obj;

	for(char key = 'a'; key != 'q'; key = cv::waitKey(40)){
		blobs.Definir_limites_separation(&hsv);
		Cameras::Find_blobs(&img_input, &blobs, &detected_obj, &horizontal_obj, &vertical_obj, &size_obj);
		cv::imshow("Blob", blobs.Get_img_blobs());
	}

	cv::imwrite("img_blobs.png", blobs.Get_img_blobs());
	reco.Set_img(blobs.Get_img_blobs());
	cv::Mat img_pipeline = reco.Trouver_ligne_principale(true, &detected_pipe, &angle_pipe, &distance_pipe);
	if(detected_obj){
		cout << "Object detected :" << endl;
		cout << "\tSize = " << size_obj << endl;
		cout << "\tX = " << horizontal_obj << endl;
		cout << "\tY = " << vertical_obj << endl;
		if(detected_pipe){
			cout << "\tPipeline detected :" << endl;
			cout << "\t\tangle = " << angle_pipe << endl;
			cout << "\t\tdistance = " << distance_pipe << endl;
		}
		else{
			cout << "\tNo pipeline detected :" << endl;
		}
	}
	else{
		cout << "No bject detected :" << endl;
	}
}
