/*
 * @(#)		ComPython.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Execute Python script instead of C++ code
 */

#ifndef COMPYTHON
#define COMPYTHON

#ifdef ENABLE_PYTHON
	#include <Python.h>
#endif

#include <iostream>
#include "ComThread.h"

class ComPython : public ComThread{

public:

	ComPython();
	~ComPython();

	void Connect_python(			// Connect to a Python script
		std::string script_name,		// Script name (without .py) in src/parts/python
		std::string class_name);		// Class to use in this script
	void Disconnect_python();		// Close Python (destructor calls it if you forget ...)

private:

	#ifdef ENABLE_PYTHON
		PyObject *pObject;
		PyObject *pFunc;
	#endif
	bool connected;
	PFloatMap input_keys, output_keys;

protected:

	void Link_input_python(			// Link input from Betterave AND output to Python (do not use Link_input)
		std::string key,			// Topic name
		float *p_float);			// Pointer to associated variable

	void Link_output_python(		// Link output to Betterave AND input from Python (do not use Link_output)
		std::string key,			// Topic name
		float *p_float);			// Pointer to associated variable

	void Send_to_python();			// Publish input variables to Python
	void Receive_from_python();		// Refresh output variables from Python
	void Job_python();			// Call Job() method of Python script

};

#endif
