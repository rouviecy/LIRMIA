/*
 * @(#)		ComJava.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Execute Java code instead of C++ code
 */

#ifndef COMJAVA
#define COMJAVA

#ifdef ENABLE_JAVA
	#include <jni.h>
#endif

#include "ComThread.h"
#include "CoreJava.h"

typedef struct{
	#ifdef ENABLE_JAVA
		jclass java_class;
		jobject java_instance;
		jmethodID java_job;
	#endif
}JOBJECTS_STRUCT;

class ComJava : public ComThread{

public:

	ComJava();
	~ComJava();

	void Connect_java(			// Connect to a Java class
		CoreJava* corejava,			// Link to CoreJava
		std::string full_class_name);		// "Java_part/..."
	void Disconnect_java();			// Close Java (destructor calls it if you forget ...)

private:

	COREJAVA* jstr;
	JOBJECTS_STRUCT* jobj;
	JOBJECTS_STRUCT* Build_objects();
	bool connected;
	std::string full_class_name;
	PFloatMap input_keys, output_keys;

	void On_start();

protected:

	void Link_input_java(			// Link input from Betterave AND output to Java (do not use Link_input)
		std::string key,			// Topic name
		float *p_float);			// Pointer to associated variable

	void Link_output_java(			// Link output to Betterave AND input from Java (do not use Link_output)
		std::string key,			// Topic name
		float *p_float);			// Pointer to associated variable

	void Send_to_java();			// Publish input variables to Java
	void Receive_from_java();		// Refresh output variables from Java
	void Job_java();			// Call Job() method of Java script

};

#endif
