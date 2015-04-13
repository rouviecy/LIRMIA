#include "ComPython.h"

using namespace std;

ComPython::ComPython() : ComThread(){
	connected = false;
}

ComPython::~ComPython(){
	Disconnect_python();
}

void ComPython::Connect_python(string script_name, string class_name){

	#ifdef ENABLE_PYTHON
		// Verify it is not already connected
		if(connected){
			cout << "[Warning] Python is already connected !" << endl;
			return;
		}

		// Initialize and include current path to PYTHONPATH
		Py_Initialize();
		PyRun_SimpleString("import sys");
		PyRun_SimpleString("sys.path.insert(0, '../src/parts/python')");

		// Load file
		PyObject *pName = PyUnicode_FromString(script_name.c_str());
		PyObject *pModule = PyImport_Import(pName);
		Py_DECREF(pName);
		if(pModule == NULL){
			cout << "[Error] Failed to load python script \"" + script_name + "\"" << endl;
			return;
		}

		// Load class
		PyObject *pClass = PyObject_GetAttrString(pModule, class_name.c_str());
		Py_DECREF(pModule);
		if(!pClass){
			cout << "[Error] Failed to load python class \"" + class_name + "\"" << endl;
			return;
		}

		// Instantiate object (without argument)
		pObject = PyObject_CallObject(pClass, NULL);
		Py_DECREF(pClass);
		if(pObject == NULL || !PyObject_IsInstance(pObject, pClass)){
			cout << "[Error] Failed to instantiate python class \"" + class_name + "\"" << endl;
			return;
		}

		// Load Job method
		pFunc = PyObject_GetAttrString(pObject, "Job");
		if(!pFunc){
			cout << "[Error] Failed to load Job function in Python script \"" + class_name + "\"" << endl;
			return;
		}

		connected = true;

	#else
		cout << "[Warning] You are trying to connect Python script without Python enabled (see CMakeLists.txt)" << endl;
	#endif 
}

void ComPython::Disconnect_python(){
	#ifdef ENABLE_PYTHON
		if(connected){
			Py_DECREF(pFunc);
			Py_DECREF(pObject);
			Py_Finalize();
			connected = false;
		}
	#endif
}

void ComPython::Link_input_python(string key, float *p_float){
	Link_input(key, p_float);
	#ifdef ENABLE_PYTHON
		if(!PyObject_HasAttrString(pObject, key.c_str())){
			cout << "[Error] Failed to link input \"" + key + "\" which does not exist in Python script for part \"" << Get_name() << endl;
			return;
		}
		input_keys[key] = p_float;
	#endif
}

void ComPython::Link_output_python(string key, float *p_float){
	Link_output(key, p_float);
	#ifdef ENABLE_PYTHON
		if(!PyObject_HasAttrString(pObject, key.c_str())){
			cout << "[Error] Failed to link output \"" + key + "\" which does not exist in Python script for part \"" << Get_name() << endl;
			return;
		}
		output_keys[key] = p_float;
	#endif
}

void ComPython::Send_to_python(){
	#ifdef ENABLE_PYTHON
		for(PFloatMap::iterator it = input_keys.begin(); it != input_keys.end(); ++it){
			PyObject *pValue = PyFloat_FromDouble((double) *(it->second));
			PyObject_SetAttrString(pObject, (it->first).c_str(), pValue);
			Py_DECREF(pValue);
		}
	#endif
}

void ComPython::Receive_from_python(){
	#ifdef ENABLE_PYTHON
		for(PFloatMap::iterator it = output_keys.begin(); it != output_keys.end(); ++it){
			PyObject *pValue = PyObject_GetAttrString(pObject, (it->first).c_str());
			*(it->second) = (float) PyFloat_AsDouble(pValue);
			Py_DECREF(pValue);
		}
	#endif
}

void ComPython::Job_python(){
	#ifdef ENABLE_PYTHON
		PyObject_CallObject(pFunc, NULL);
	#endif	
}
