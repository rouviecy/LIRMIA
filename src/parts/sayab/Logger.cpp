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
			<< setfill('0') << setw(2) << now->tm_sec << "--Sayab.log";
	log_file.open(log_name.str());
	string header = "t\tstate\tx\ty\tthz\tvthz\tyawref\tuw\tmotor\trudder\tbow\tey1\tey2\talfab1\talfab2\tgpsx\tgpsy\tgpslat\tgpslon";
	log_file << header << "\n";
	last_t_save = -1.;
}

Logger::~Logger(){
	log_file.close();
}

void Logger::On_start(){}

void Logger::IO(){
	Link_input("fsm_state",		COMINT,		1, &fsm_state);
	Link_input("t",			COMFLOAT,	1, &t);
	Link_input("xy",		COMFLOAT,	2, xy);
	Link_input("thz",		COMFLOAT,	1, &thz);
	Link_input("vthz",		COMFLOAT,	1, &vthz);
	Link_input("yawref",		COMFLOAT,	1, &yawref);
	Link_input("uw",		COMFLOAT,	1, &uw);
	Link_input("motor",		COMFLOAT,	1, &motor);
	Link_input("rudder",		COMFLOAT,	1, &rudder);
	Link_input("bow_thruster",	COMFLOAT,	1, &bow_thruster);
	Link_input("ey1",		COMFLOAT,	1, &ey1);
	Link_input("ey2",		COMFLOAT,	1, &ey2);
	Link_input("alfab1",		COMFLOAT, 	1, &alfab1);
	Link_input("alfab2",		COMFLOAT,	1, &alfab2);
	Link_input("gps_xy",		COMFLOAT,	2, gps_xy);
	Link_input("gps_lat",		COMFLOAT,	1, &gps_lat);
	Link_input("gps_lon",		COMFLOAT,	1, &gps_lon);
//	Link_input("tim",		COMFLOAT,	1, &tim);
//	Link_input("msgmod",		COMFLOAT, 	4, &msgmod);
}

void Logger::Job(){
	Critical_receive();
	if(t < 1.){return;}
	stringstream new_line;
	new_line.setf(ios::fixed, ios::floatfield);
	new_line.setf(ios::showpoint);
	new_line.precision(2);
	new_line	<< t		<< "\t" << State_machine::Decode_state_str(fsm_state)	<< "\t"
			<< xy[0]	<< "\t" << xy[1]	<< "\t"	<< thz			<< "\t"
			<< vthz		<< "\t" << yawref       << "\t"	<< uw			<< "\t"
			<< motor        << "\t" << rudder	<< "\t"	<< bow_thruster	  	<< "\t"
			<< ey1		<< "\t" << ey2		<< "\t" << alfab1		<< "\t"
			<< alfab2	<< "\t" << gps_xy[0]	<< "\t" << gps_xy[1] 		<< "\t"
			<< gps_lat*10000<< "\t"	<< gps_lon*10000<< "\t";
	log_file << new_l << State_machine::Decode_state_str(fsm_state) ine.str() << "\n";
	if(t - last_t_save > 10){
		log_file.flush();
		last_t_save = t;
	}
}
