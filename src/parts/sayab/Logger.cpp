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
	string header = "t\tstate\t\t\t\tx\ty\tthz\tmotor\trudder\tbow1\tbow2";
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
	Link_input("motor",		COMFLOAT,	1, &motor);
	Link_input("rudder",		COMFLOAT,	1, &rudder);
	Link_input("bow_thruster",	COMFLOAT,	1, &bow_thruster);
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
			<< motor	<< "\t" << rudder	<< "\t" << bow_thruster		<< "\t";
	log_file << new_line.str() << "\n";
	if(t - last_t_save > 5){
		log_file.flush();
		last_t_save = t;
	}
}
