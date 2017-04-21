#include "Logger.h"

using namespace std;

Logger::Logger() : ComThread(){
	time_t t_now = time(0);
	struct tm* now = localtime(&t_now);
	stringstream log_name;
	log_name	<< "./test/"
			<< (now->tm_year + 1900) << "-"
			<< setfill('0') << setw(2) << (now->tm_mon + 1) << "-"
			<< setfill('0') << setw(2) << now->tm_mday << "--"
			<< setfill('0') << setw(2) << now->tm_hour << ":"
			<< setfill('0') << setw(2) << now->tm_min << ":"
			<< setfill('0') << setw(2) << now->tm_sec << "--LIRMIA.log";
	log_file.open(log_name.str());
//pdbouy
	string header = "t\tstate\tz\tvz\tzref\tmodema\tmodemb\tmodemc\tmotor1\tmotor2\tmotor3\tmotor4\tang1\tang2\tdist1\tdist2\thor1\thor2\tvert1\tvert2\tmotor4\tuzpdc\tkpcz\tkdcz\tIz\tgcz";
//backstepping
	//string header = "t\tstate\tz\tvz\tzref\tmodema\tmodemb\tmodemc\tmotor1\tmotor2\tmotor3\tmotor4\tang1\tang2\tdist1\tdist2\thor1\thor2\tvert1\tvert2\tuzb\talfabz1\talfabz2\tIz\tgcz";
//nlpdz
	//string header = "t\tstate\tz\tvz\tzref\tmodema\tmodemb\tmodemc\tmotor1\tmotor2\tmotor3\tmotor4\tang1\tang2\tdist1\tdist2\thor1\thor2\tvert1\tvert2\tuzpds\tkpz\tkdz\tdpz\tbpz\tmupz\tddz\tbdz\tmudz\tIz\tgcz";
//predictor filter
	//string header = "t\tstate\tz\tvz\tzref\tmodema\tmodemb\tmodemc\tmotor1\tmotor2\tmotor3\tmotor4\tang1\tang2\tdist1\tdist2\thor1\thor2\tvert1\tvert2\tpitch\tvpitch\tuzpf\tg1\tk1\tk2\tk3\tf1\th\tIz\tgcz";

	log_file << header << "\n";
	last_t_save = -1.;
}

Logger::~Logger(){
	log_file.close();
}

void Logger::On_start(){}

void Logger::IO(){
	Link_input("fsm_state",			COMINT,		1, &fsm_state);
	Link_input("t",				COMFLOAT,	1, &t);
	Link_input("xyz",                       COMFLOAT,       3, xyz);
        Link_input("zref",             		COMFLOAT, 	1, &zref);
        Link_input("vxyz",             		COMFLOAT, 	1, &vxyz);
        Link_input("vz",               		COMFLOAT, 	1, &vz);
	Link_input("thxyz",	            	COMFLOAT, 	3, thxyz);
        Link_input("vthxyz",    	       	COMFLOAT, 	3, vthxyz);
        Link_input("mywxy",            		COMFLOAT, 	2, mywxy);

        Link_input("uzpdc",	        	COMFLOAT, 	1, &uzpdc);
        Link_input("uzb",              		COMFLOAT, 	1, &uzb);
        Link_input("uzpds",            		COMFLOAT, 	1, &uzpds);
        Link_input("kpcz",             		COMFLOAT, 	1, &kpcz);
        Link_input("kdcz",             		COMFLOAT, 	1, &kdcz);
        Link_input("kpz",              		COMFLOAT, 	1, &kpz);
        Link_input("kdz",              		COMFLOAT, 	1, &kdz);
	Link_input("motor",			COMFLOAT,	4, motor);
	Link_input("uzpf",	              	COMFLOAT, 	1, &uzpf);

	Link_input("cam_pipeline_angle",	COMFLOAT,	2, cam_pipeline_angle);
	Link_input("cam_pipeline_distance",	COMFLOAT,	2, cam_pipeline_distance);
	Link_input("cam_detect_horizontal",	COMFLOAT,	2, cam_detect_horizontal);
	Link_input("cam_detect_vertical",	COMFLOAT,	2, cam_detect_vertical);

        Link_input("alfabz1",          		COMFLOAT, 	1, &alfabz1);
        Link_input("alfabz2",          		COMFLOAT, 	1, &alfabz2);
        Link_input("dpz",              		COMFLOAT, 	1, &dpz);
        Link_input("bpz",              		COMFLOAT, 	1, &bpz);
        Link_input("mupz",             		COMFLOAT, 	1, &mupz);
        Link_input("ddz",              		COMFLOAT, 	1, &ddz);
        Link_input("bdz",              		COMFLOAT, 	1, &bdz);
        Link_input("mudz",             		COMFLOAT, 	1, &mudz);
	Link_input("Iz",			COMFLOAT,	1, &Iz);
	Link_input("gcz",			COMFLOAT,	1, &gcz);
	Link_input("g1",	               	COMFLOAT, 	1, &g1);
        Link_input("k1",               		COMFLOAT, 	1, &k1);
        Link_input("k2",               		COMFLOAT, 	1, &k2);
        Link_input("k3",               		COMFLOAT, 	1, &k3);
        Link_input("f1",               		COMFLOAT, 	1, &f1);
	Link_input("h",				COMFLOAT, 	1, &h);

}

void Logger::Job(){
	Critical_receive();
	if(t < 1.){return;}
	stringstream new_line;
	new_line.setf(ios::fixed, ios::floatfield);
	new_line.setf(ios::showpoint);
	new_line.precision(2);
	new_line	<< t						<< "\t"
			<< State_machine::Decode_state_str(fsm_state)	<< "\t"
			<<	xyz[2]          << "\t" <<      vxyz[2]      << "\t"
			<<	zref		<< "\t" <<	mywxy[0]     << "\t" <<	     mywxy[1]	     << "\t" <<      mywxy[2]        << "\t"
			<< 	motor[0]	<< "\t"	<<	motor[1]     << "\t" <<      motor[2]        << "\t" <<      motor[3]	     << "\t"
			<< cam_pipeline_angle[0]<< "\t" << cam_pipeline_angle[1]<< "\t" << cam_pipeline_distance[0]<< "\t"<< cam_pipeline_distance[1]<< "\t"
                        << cam_detect_horizontal[0]<< "\t"<< cam_detect_horizontal[1]<< "\t"<< cam_detect_vertical[0]<< "\t"<< cam_detect_vertical[1]<< "\t"
			<<   uzpdc	<< "\t" <<	kpcz		<< "\t" <<	kdcz	     << "\t" //PDBUOY
//			<<     uzb     << "\t" <<    alfabz1   << "\t" <<   alfabz2   << "\t"  //BACKSTEPPINGZ
//			<< uzpds << "\t" << kpz << "\t" << kdz<< "\t"  //NLPDZ
//			<< dpz << "\t" << bpz << "\t" << mupz << "\t"  << ddz << "\t" << bdz << "\t" << mudz << "\t"
//			<<   thxyz[1] <<"\t"<<  vthxyz[1]  <<"\t"<<  uzpf  <<"\t"<< g1 <<"\t"<< k1 <<"\t"<< k2 <<"\t"<< k3 <<"\t"<< f1 <<"\t" << h << "\t"  //PREDICTOR FILTER Z
			<< Iz << "\t" <<    gcz     << "\t";


	log_file << new_line.str() << "\n";
	if(t - last_t_save > 5){
		log_file.flush();
		last_t_save = t;
	}
}
