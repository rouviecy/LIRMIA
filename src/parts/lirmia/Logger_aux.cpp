#include "Logger_aux.h"

using namespace std;

Logger_aux::Logger_aux() : ComThread(){
	time_t t_now = time(0);
	struct tm* now = localtime(&t_now);
	stringstream log_name;
	log_name	<< "./test/"
			<< (now->tm_year + 1900) << "-"
			<< setfill('0') << setw(2) << (now->tm_mon + 1) << "-"
			<< setfill('0') << setw(2) << now->tm_mday << "--"
			<< setfill('0') << setw(2) << now->tm_hour << ":"
			<< setfill('0') << setw(2) << now->tm_min << ":"
			<< setfill('0') << setw(2) << now->tm_sec << "--LIRMIA_aux.log";
	log_file.open(log_name.str());
//pyaw
	string header = "t\tstate\tyaw\tvyaw\tyawref\tmotor1\tmotor2\tmotor3\tmotor4\tang1\tang2\tdist1\tdist2\thor1\thor2\tvert1\tvert2\tIz";
//backsteppingyaw
	//string header = "t\tstate\tyaw\tvyaw\tyawref\tmotor1\tmotor2\tmotor3\tmotor4\tang1\tang2\tdist1\tdist2\thor1\thor2\tvert1\tvert2\tuwb\talfabw1\talfabw2\tIz";
//nlpdyaw
	//string header = "t\tstate\tyaw\tvyaw\tyawref\tmotor1\tmotor2\tmotor3\tmotor4\tang1\tang2\tdist1\tdist2\thor1\thor2\tvert1\tvert2\tuwpds\tkpw\tkdw\tdpw\tbpw\tmupw\tddw\tbdw\tmudw\tIz";


	log_file << header << "\n";
	last_t_save = -1.;
}

Logger_aux::~Logger_aux(){
	log_file.close();
}

void Logger_aux::On_start(){}

void Logger_aux::IO(){
	Link_input("fsm_state",			COMINT,		1, &fsm_state);
	Link_input("t",				COMFLOAT,	1, &t);
	Link_input("thxyz",	            	COMFLOAT, 	3, thxyz);
        Link_input("vthxyz",    	       	COMFLOAT, 	3, vthxyz);
        Link_input("yawref",           		COMFLOAT, 	1, &yawref);

        Link_input("uwb",        	       	COMFLOAT, 	1, &uwb);
        Link_input("uwpds",            		COMFLOAT, 	1, &uwpds);
	Link_input("kpw",              		COMFLOAT, 	1, &kpw);
        Link_input("kdw",              		COMFLOAT, 	1, &kdw);
	Link_input("motor",			COMFLOAT,	4, motor);

	Link_input("cam_pipeline_angle",	COMFLOAT,	2, cam_pipeline_angle);
	Link_input("cam_pipeline_distance",	COMFLOAT,	2, cam_pipeline_distance);
	Link_input("cam_detect_horizontal",	COMFLOAT,	2, cam_detect_horizontal);
	Link_input("cam_detect_vertical",	COMFLOAT,	2, cam_detect_vertical);

        Link_input("alfabw1",          		COMFLOAT, 	1, &alfabw1);
        Link_input("alfabw2",          		COMFLOAT, 	1, &alfabw2);
        Link_input("dpw",              		COMFLOAT, 	1, &dpw);
        Link_input("bpw",              		COMFLOAT, 	1, &bpw);
        Link_input("mupw",             		COMFLOAT, 	1, &mupw);
        Link_input("ddw",              		COMFLOAT, 	1, &ddw);
        Link_input("bdw",              		COMFLOAT, 	1, &bdw);
        Link_input("mudw",             		COMFLOAT, 	1, &mudw);
	Link_input("Iz",			COMFLOAT,	1, &Iz);
}

void Logger_aux::Job(){
	Critical_receive();
	if(t < 1.){return;}
	stringstream new_line;
	new_line.setf(ios::fixed, ios::floatfield);
	new_line.setf(ios::showpoint);
	new_line.precision(2);
	new_line	<< t						<< "\t"
			<< State_machine::Decode_state_str(fsm_state)	<< "\t"
			<< thxyz[2]	<< "\t"	<<	vthxyz[2]	<< "\t" <<  yawref  << "\t"
			<< motor[0]	<< "\t"	<<	motor[1]	<< "\t"	<< 	motor[2]     << "\t" <<      motor[3]	     << "\t"
			<< cam_pipeline_angle[0]<< "\t" << cam_pipeline_angle[1]<< "\t" << cam_pipeline_distance[0]<< "\t"<< cam_pipeline_distance[1]<< "\t"
                        << cam_detect_horizontal[0]<< "\t"<< cam_detect_horizontal[1]<< "\t"<< cam_detect_vertical[0]<< "\t"<< cam_detect_vertical[1]<< "\t"
//			<< uwb	   <<  alfabw1    << "\t" <<    alfabw2   << "\t" //BACKSTEPPING
//			<<   uwpds   << "\t" <<	kpw   << "\t" <<   kdw	 << "\t" //NLPDYAW
//			<<   dpw     << "\t" <<  bpw   << "\t" <<   mupw  << "\t"  <<   ddw  << "\t" <<    bdw       << "\t" <<      mudw   << "\t"
			<<    Iz     << "\t";


	log_file << new_line.str() << "\n";
	if(t - last_t_save > 5){
		log_file.flush();
		last_t_save = t;
	}
}
