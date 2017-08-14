#include "../interfaces/Camera_client.h"

using namespace std;

int main(int argc, char* argv[]){
	#if not defined(ENABLE_TCP) or not defined(ENABLE_TCPCAM)
		cout << "[Error] You need to activate TCP and TCPCAM to use remote camera" << endl;
		return;
	#endif

	Camera_client cli;

	#ifdef ENABLE_CAM1
		cli.Subscribe_cam("R2D2 camera 1",	argv[1], 6243, 800, 150);
		cli.Subscribe_cam("R2D2 blobs 1",	argv[1], 6245, 1200, 150);
		cli.Subscribe_cam("image",		argv[1], 6247, 1200, 400);
		cli.Subscribe_cam("polar",		argv[1], 6248, 800, 400);
	#endif

	usleep(500000);
	cli.Show_video();
}
