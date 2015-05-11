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
	string header = "t\tstate\t\t\t\tx\ty\tz\tvx\tvy\tvz\tthx\tthy\tthz\tmotor1\tmotor2\tmotor3\tmotor4";
	log_file << header << "\n";
	last_t_save = -1.;
}

Logger::~Logger(){
	log_file.close();
}

void Logger::On_start(){}

void Logger::IO(){
	Link_input("fsm_state",	COMINT,		1, &fsm_state);
	Link_input("t",		COMFLOAT,	1, &t);
	Link_input("xyz",	COMFLOAT,	3, xyz);
	Link_input("vxyz",	COMFLOAT,	3, vxyz);
	Link_input("thxyz",	COMFLOAT,	3, thxyz);
	Link_input("vthxyz",	COMFLOAT,	3, vthxyz);
	Link_input("motor",	COMFLOAT,	4, motor);
}

void Logger::Job(){
	Critical_receive();
	if(t < 1.){return;}
	stringstream new_line;
	new_line.setf(ios::fixed, ios::floatfield);
	new_line.setf(ios::showpoint);
	new_line.precision(2);
	new_line	<< t		<< "\t" << State_machine::Decode_state_str(fsm_state) << "\t"
			<< xyz[0]	<< "\t" << xyz[1]	<< "\t" << xyz[2]	<< "\t"
			<< vxyz[0]	<< "\t" << vxyz[1]	<< "\t" << vxyz[2]	<< "\t"
			<< thxyz[0]	<< "\t" << thxyz[1]	<< "\t" << thxyz[2]	<< "\t"
			<< motor[0]	<< "\t" << motor[1]	<< "\t" << motor[2]	<< "\t"		<< motor[3];
	log_file << new_line.str() << "\n";
	if(t - last_t_save > 5){
		log_file.flush();
		last_t_save = t;
	}
}
