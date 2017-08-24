#include "Sonar.h"

using namespace std;

Sonar::Sonar() : ComThread(){
	obst_xy[0] = 42.;
	obst_xy[1] = 42.;
	mtSendV=0;
	request = 'Z';

}

Sonar::~Sonar(){}

void Sonar::On_start(){
	//abc=Add_flux(6247);
	fready=0;
	int flag1=0;
		while(flag1==0){
			char* answer1 = serial_s->Serial_read2(22);
			if(answer1[10]==4){
				flag1=1;
				cout<<"mtAlive"<<endl;
			}
			else{
				cout<<+answer1[0]<<"  "<<+answer1[10]<<"  "<<+answer1[21]<<"  "<<+answer1[22]<<endl;
				usleep(500000);
			}
		}
		obst_xy[0] = Get_version(serial_s);
		usleep(1000);
		obst_xy[0] = Get_user(serial_s);
		obst_xy[0] = Config_sonar(serial_s);
}

void Sonar::IO(){
	//Link_output("obst_xy",	COMFLOAT, 2, obst_xy);
	Link_output("sData", 	COMFLOAT,  1,&sData);
	Link_output("fready", 	COMFLOAT,  1,&fready);
}



void Sonar::Job(){
	
	#ifdef ENABLE_SERIAL_RS232_SONAR
		/*int flag1=0;
		while(flag1==0){
			char* answer1 = serial_s->Serial_read2(22);
			if(answer1[10]==4){
				flag1=1;
				cout<<"mtAlive"<<endl;
			}
			else{
				cout<<+answer1[0]<<"  "<<+answer1[10]<<"  "<<+answer1[21]<<"  "<<+answer1[22]<<endl;
				usleep(500000);
			}
		}
		obst_xy[0] = Get_version(serial_s);
		usleep(1000);
		serial_s->Flush();
		obst_xy[0] = Get_user(serial_s);
		serial_s->Flush();
		obst_xy[0] = Config_sonar(serial_s);*/
		
		//serial_s->Flush();
		obst_xy[0] = Ask_data(serial_s);
		//cout<<"bandera 1"<<endl;
		usleep(50000);
		sData = Get_data(serial_s,sData);
		usleep(1000);
		for (int i=0;i<245;i++){
			//if(sData[i]>255){
				//break;		
			//}				
			//cout<<sData[i]<<" ";	
		}
		fready=0;
		//cout<<"bandera 5"<<endl;
		//cout<<sData[40]<<" "<<sData[41]<<" "<<sData[242]<<endl;
		usleep(50000);
		sData = Get_data(serial_s,sData);
		usleep(1000);
		//cout<<sData[40]<<" "<<sData[41]<<" "<<sData[242]<<endl;
		serial_s->Flush();
		//abc=Add_flux(6247);
		//abc=Send_tcp(sData,6247);
		Critical_send();
	#endif	
	
}


float Sonar::Get_version(Serial* serial){
	float result = -1;
	unsigned char mtS[15]={64,48,48,48,56,8,0,255,2,3,23,128,2,10};
	serial->Lock();
	serial->Flush();
	for(int i=0;i<14;i++){
	mtSendV=mtS[i];
	serial->Serial_write(&mtSendV,1);
	}
	usleep(10000);
	int flag1=0;
	while(flag1==0){
		char* answer1 = serial_s->Serial_read2(25);
		if(answer1[10]==1){
			flag1=1;
			cout<<"mtVersionData"<<endl;
		}
		else{	
			for(int i=0;i<25;i++){
				cout<<+answer1[i]<<" ";
			}
			cout<<endl;
			usleep(500000);
		}
		try{result = stof(string(answer1));}
		catch(std::exception const & e){}
	}
	serial->Unlock();
	return result;
}



float Sonar::Get_user(Serial* serial){
	float result = -1;
	unsigned char mtB[14]={64,48,48,48,56,8,0,255,2,3,24,128,2,10};
	serial->Lock();
	for(int i=0;i<14;i++){
	mtSendV=mtB[i];
	serial->Serial_write(&mtSendV,1);
	}
	usleep(101000);
	//usleep(1000000);
	int flag1=0;
	while(flag1==0){
		char* answer1 = serial_s->Serial_read2(372);
		usleep(100000);	
		if(answer1[10]==06){
			flag1=1;
			cout<<"mtBBuserData"<<endl;
		}
		else{
			for(int i=0;i<372;i++){
				cout<<+answer1[i]<<" ";
			}
			cout<<endl;			
			usleep(50000);

			}
		try{result = stof(string(answer1));}
		catch(std::exception const & e){}
	}
	serial->Unlock();
	return result;
}
float Sonar::Config_sonar(Serial* serial){
	float result = -1;
	unsigned char mtH[82]={64,48,48,52,67,76,0,255,2,71,19,128,2, 1,  6,35,17,209, 92,143,0,209,92,143,0,147, 89,102,92,147, 89,102,92,100,0, 20,0,64,6,192,18,30,140, 84, 84,125,0,125,0,25,16,11,0, 144,1,244,1,100,0,64,6,1,0,0,0,80,81,9,8, 84, 84,0,0, 90,0,125,0,0,0,0,0,10};//0.9°
	serial->Lock();
	
	for(int i=0;i<82;i++){
	mtSendV=mtH[i];
	serial->Serial_write(&mtSendV,1);
	}
	usleep(100000);
	int flag1=0;
	while(flag1==0){
		char* answer1 = serial_s->Serial_read2(22);
		if(answer1[10]==4 && (answer1[20]==146 || answer1[20]==154 || answer1[20]==-110 || answer1[20]==-102)){
			flag1=1;
			cout<<"mtVersionData con datos"<<endl;
		}
		else{
			for(int i=0;i<22;i++){
				//cout<<+answer1[i]<<" ";
			}
			//cout<<endl;
			usleep(500000);
			}
		try{result = stof(string(answer1));}
		catch(std::exception const & e){}
	}
	serial->Unlock();
	return result;
}

float Sonar::Ask_data(Serial* serial){
	float result = -1;
	unsigned char mtD[18]={64,48,48,48,67,12,0,255,2,7,25,128,2,0,0,0,0,10};
	serial->Lock();
	for(int i=0;i<18;i++){
	mtSendV=mtD[i];
	serial->Serial_write(&mtSendV,1);	
	}
	serial->Unlock();
	return result;
}

float* Sonar::Get_data(Serial* serial, float* data){
	float data2[246];
	char* answer1 = serial_s->Serial_read2(245);	
	answer1[245]=0;	
	for (int i=0;i<245;i++){
		//cout<<"bandera 2"<<endl;
		data2[i]=answer1[i];
		 //data[i]=int(answer1[i]);
		//cout<<+answer1[i]<<" "<<i<<" ";		
		//cout<<data2[i]<<" ";	
	}		
	//cout<<data2[40]<<" "<<data2[41]<<" "<<data2[240]<<" "<<endl;
	//cout<<endl;
	return data2;
}
void Sonar::Set_serial_x(Serial* serial){this->serial_x = serial;}
void Sonar::Set_serial_y(Serial* serial){this->serial_y = serial;}
void Sonar::Set_serial_s(Serial* serial){this->serial_s = serial;}

//Funciones de servidor
bool Sonar::Add_flux(int port){
	#ifdef ENABLE_TCP
		if(tcp_servers.count(port) > 0){
			cout << "[Error] Trying to add TCP sonar flux to port " << to_string(port) << " which is already used" << endl;
			return false;
		}
		TCP_server* new_server = new TCP_server();
		if(!new_server->Configure(port)){
			cout << "[Error] Failed to open TCP sonar server on port " << to_string(port) << endl;
			return false;
		}
		cout << "Waiting client for TCP sonar on port " << to_string(port) << " ..." << endl;
		while(new_server->Get_nb_clients() == 0){
			usleep(200000);
		}
		cout << "Client for TCP  sonar connected on port " << to_string(port) << endl;
		tcp_servers[port] = new_server;
		return true;
	#else
		cout << "[Warning] Trying to open TCP sonar server with TCP disabled" << endl;
		return false;
	#endif
}


bool Sonar::Send_tcp(int* sData, int port){
	#ifdef ENABLE_TCP
		if(tcp_servers.count(port) == 0){
			cout << "[Error] Cannot send image to TCP port " << to_string(port) << " which is not opened" << endl;
			return false;
		}
		/*cv::Mat img_mini;
		std::vector <unsigned char> msg;
		cv::resize(img, img_mini, cv::Size(320, 240));
		cv::imencode(".jpg", img_mini, msg, encode_params);*/
		//tcp_servers[port]->Send(to_string(msg.size()));
		unsigned char msg_char[245];//[msg.size()];
		for(int i = 0; i < 245; i++){
			msg_char[i] = char(sData[i]);
			//cout << " " << msg_char[i];	
		}
		cout  << endl;
		tcp_servers[port]->Direct_send(msg_char, 245);
		return true;
	#endif
}
