/*
 * @(#)		ComThread.h
 * @version	2.1
 * @autor	C. Rouvière
 */

/**
 * Thread with publisher-subscriber communication
 */

#ifndef COMTHREAD
#define COMTHREAD

#include <unistd.h>
#include <thread>
#include "Share.h"

class ComThread{

public:

	ComThread();						// Create the thread

	void Link(						// Link to shared memory
		Share *s);						// Pointer to share object

	void Launch();						// Begin the job

	void Join();						// Wait the end of the thread

	void Set_freq(						// Setup frequency thread
		int dt_microseconds);					// Period between two jobs in μs (-1 for no loop)

	void Set_name(						// Give a string name to the thread
		std::string name);					// New name

	std::string Get_name();					// Return the name of the object

	float Get_freq();					// Return the frequency of the thread

	std::map <std::string, std::string> Get_inputs();	// Return inputs map with their data types

	std::map <std::string, std::string> Get_outputs();	// Return outputs map with their data types

private:

	std::thread thr;
	int dt_microseconds;
	bool job_go_on;
	std::string name;
	
	Share *s;
	PVoidMap critical_input, critical_output;

	std::map <std::string, std::string> input_types, output_types;
	
	void Loop_job();

protected:

	virtual void On_start() = 0;	// Overwrite : called on thread start
	virtual void Job() = 0;		// Overwrite : job for the thread
	virtual void IO() = 0;		// Overwrite : input and output links

	void Link_input(		// Subscribe to a topic
		std::string key,		// Topic name
		T_DATA data_type,		// Type of data
		int size,			// Size of data > 0
		void *p_data);			// Pointer to associated variable

	void Link_output(		// Publisher of a topic
		std::string key,		// Topic name
		T_DATA data_type,		// Type of data
		int size,			// Size of data > 0
		void *p_data);			// Pointer to associated variable

	void Critical_receive();	// Refresh variables for subscribed topics
	void Critical_send();		// Publish variables

};

#endif
