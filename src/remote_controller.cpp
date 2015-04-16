#include "interfaces/TCP_client.h"

using namespace std;

int main(){
	TCP_client tcp_client;
	tcp_client.Configure("127.0.0.1", 4242);
	for(int i = 0; i < 5; i++){
		tcp_client.Send("test");
		usleep(1000000);
	}
	tcp_client.Send("bye");
	usleep(1000000);
	tcp_client.Close();
}
