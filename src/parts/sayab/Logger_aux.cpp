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
			<< setfill('0') << setw(2) << now->tm_sec << "--Sayab_aux.log";
	log_file.open(log_name.str());

//forward control
	string header = "t\tstate\tx\ty\tgpslat\tgpslon\tgpsbearing\tgpsreflat\tgpsreflon\tdistance\tmotor\trudder\tbow\tufpd\tkpf\tkdf";

	log_file << header << "\n";
	last_t_save = -1.;
}

Logger_aux::~Logger_aux(){
	log_file.close();
}

void Logger_aux::On_start(){}

void Logger_aux::IO(){
	Link_input("fsm_state",		COMINT,		1, &fsm_state);
	Link_input("t",			COMFLOAT,	1, &t);
	Link_input("xy",		COMFLOAT,	2, xy);
	Link_input("gps_lat",		COMFLOAT,	1, &gps_lat);
	Link_input("gps_lon",		COMFLOAT,	1, &gps_lon);
	Link_input("thz",		COMFLOAT,	1, &thz);
	Link_input("vthz",		COMFLOAT,	1, &vthz);
	Link_input("gpsbearing",	COMFLOAT,	1, &gpsbearing);
	Link_input("gpsreflat",		COMFLOAT,	1, &gpsreflat);
	Link_input("gpsreflon",		COMFLOAT,	1, &gpsreflon);
	Link_input("distance",		COMFLOAT,	1, &distance);
	Link_input("uf",		COMFLOAT,	1, &uf);
	Link_input("ufpd",		COMFLOAT,	1, &ufpd);
	Link_input("motor",		COMFLOAT,	1, &motor);
	Link_input("rudder",		COMFLOAT,	1, &rudder);
	Link_input("bow_thruster",	COMFLOAT,	1, &bow_thruster);
	Link_input("kpf",		COMFLOAT, 	1, &kpf);
	Link_input("kdf",		COMFLOAT,	1, &kdf);

//	Link_input("tim",		COMFLOAT,	1, &tim);
//	Link_input("msgmod",		COMFLOAT, 	4, &msgmod);
}

void Logger_aux::Job(){
	Critical_receive();
	if(t < 1.){return;}
	stringstream new_line;
	new_line.setf(ios::fixed, ios::floatfield);
	new_line.setf(ios::showpoint);
	new_line.precision(2);
	new_line	<< t		<< "\t" << State_machine::Decode_state_str(fsm_state)	<< "\t"
			<< xy[0] << "\t" << xy[1] << "\t" << gps_lat*10000 << "\t"  << gps_lon*10000 << "\t" << gpsbearing << "\t"
			<< gpsreflat*10000 << "\t" << gpsreflon*10000 << "\t" <<  distance   << "\t"
			<< motor        << "\t" << rudder	<< "\t"	<< bow_thruster	  	<< "\t"
			<<  ufpd     << "\t" << kpf  << "\t"  << kdf << "\t" ; //FORWARD

	log_file << new_line.str() << "\n";
	if(t - last_t_save > 10){
		log_file.flush();
		last_t_save = t;
	}
}
