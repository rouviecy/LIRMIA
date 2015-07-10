#include "../interfaces/Camera_client.h"

using namespace std;

int main(int argc, char* argv[]){
	#if not defined(ENABLE_TCP) or not defined(ENABLE_TCPCAM)
		cout << "[Error] You need to activate TCP and TCPCAM to use remote camera" << endl;
		return;
	#endif

	Camera_client cli;

	#ifdef ENABLE_CAM1
		cli.Subscribe_cam("Sayab camera 1",	argv[1], 5243, 800, 150);
		cli.Subscribe_cam("Sayab blobs 1",	argv[1], 5245, 1200, 150);
	#endif

	#ifdef ENABLE_CAM2
		cli.Subscribe_cam("Sayab camera 2",	argv[1], 5244, 800, 500);
		cli.Subscribe_cam("Sayab blobs 2",	argv[1], 5246, 1200, 500);
	#endif

	usleep(500000);
	cli.Show_video();
}
