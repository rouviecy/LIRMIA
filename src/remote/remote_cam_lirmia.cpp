#include "../interfaces/Camera_client.h"

using namespace std;

int main(int argc, char* argv[]){
	#if not defined(ENABLE_TCP) or not defined(ENABLE_TCPCAM)
		cout << "[Error] You need to activate TCP and TCPCAM to use remote camera" << endl;
		return;
	#endif

	Camera_client cli;

	#ifdef ENABLE_CAM1
		cli.Subscribe_cam("Lirmia camera 1",	argv[1], 4243, 800, 150);
		cli.Subscribe_cam("Lirmia blobs 1",	argv[1], 4245, 1200, 150);
	#endif

	#ifdef ENABLE_CAM2
		cli.Subscribe_cam("Lirmia camera 2",	argv[1], 4244, 800, 500);
		cli.Subscribe_cam("Lirmia blobs 2",	argv[1], 4246, 1200, 500);
	#endif

	usleep(500000);
	cli.Show_video();
}
