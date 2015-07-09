#include "interfaces/Camera_client.h"

using namespace std;

int main(int argc, char* argv[]){
	#if not defined(ENABLE_TCP) or not defined(ENABLE_TCPCAM)
		cout << "[Error] You need to activate TCP and TCPCAM to use remote camera" << endl;
		return;
	#endif

	Camera_client cli;

	#ifdef ENABLE_CAM1
		cli.Subscribe_cam("Cookie camera 1",	argv[1], 7243, 800, 150);
		cli.Subscribe_cam("Cookie blobs 1",	argv[1], 7245, 1200, 150);
	#endif

	usleep(500000);
	cli.Show_video();
}
