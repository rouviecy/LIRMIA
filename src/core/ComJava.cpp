#include "ComJava.h"

using namespace std;

ComJava::ComJava() : ComThread(){
	connected = false;
}

ComJava::~ComJava(){
	Disconnect_java();
}

JOBJECTS_STRUCT* ComJava::Build_objects(){

	#ifdef ENABLE_JAVA

		// Load java class
		jclass java_class = jstr->env->FindClass(full_class_name.c_str());
		if(!java_class){
			cout << "[Error] Failed to load Java class \"" + full_class_name + "\"" << endl;
			return NULL;
		}
		java_class = reinterpret_cast <jclass> (jstr->env->NewGlobalRef(java_class));

		// Load class constructor
		jmethodID java_class_constructor = jstr->env->GetMethodID(java_class, "<init>", "()V");
		if(!java_class_constructor){
			cout << "[Error] Failed to load constructor of Java class \"" + full_class_name + "\"" << endl;
			return NULL;
		}

		// Instantiate object
		jobject java_instance = jstr->env->NewObject(java_class, java_class_constructor);
		if(!java_instance){
			cout << "[Error] Failed to instante Java class \"" + full_class_name + "\"" << endl;
			return NULL;
		}
		java_instance = reinterpret_cast <jobject> (jstr->env->NewGlobalRef(java_instance));

		// Load Job method
		jmethodID java_job = jstr->env->GetMethodID(java_class, "Job", "()V");
		if(!java_class_constructor){
			cout << "[Error] Failed to load Job method of Java class \"" + full_class_name + "\"" << endl;
			return NULL;
		}

		// Return structure
		JOBJECTS_STRUCT* result = new JOBJECTS_STRUCT();
		result->java_class = java_class;
		result->java_instance = java_instance;
		result->java_job = java_job;
		return result;

	#else
		return NULL;
	#endif

}

void ComJava::Connect_java(CoreJava* corejava, string full_class_name){

	if(connected){
		cout << "[Warning] Java is already connected !" << endl;
		return;
	}

	#ifdef ENABLE_JAVA
		this->jstr = corejava->Get_jstr();
		this->full_class_name = full_class_name;
		if(!jstr || !corejava->Is_connected()){
			cout << "[Error] Failed to connect Java class \"" + full_class_name + "\"to JavaCore" << endl;
			return;
		}
		jobj = Build_objects();
		if(!jobj){
			cout << "[Error] Failed to initialize Java class \"" + full_class_name + "\"" << endl;
			return;
		}
		connected = true;
	#endif
}

void ComJava::Disconnect_java(){
	#ifdef ENABLE_JAVA
		if(connected){
			jstr->env->DeleteGlobalRef(jobj->java_class);
			jstr->env->DeleteGlobalRef(jobj->java_instance);
			jstr->jvm->DetachCurrentThread();
			connected = false;
		}
	#endif
}

void ComJava::On_start(){
	#ifdef ENABLE_JAVA
		int env_status = this->jstr->jvm->GetEnv((void **) &(this->jstr->env), JNI_VERSION_1_6);
		if(env_status == JNI_EDETACHED)		{this->jstr->jvm->AttachCurrentThread((void **) &(this->jstr->env), NULL);}
		else if(env_status == JNI_EVERSION)	{cout << "[Error] Java Env (version issue)" << endl; return;}
	#endif
}

void ComJava::Link_input_java(string key, float *p_float){
	Link_input(key, p_float);
	#ifdef ENABLE_JAVA
		if(!jstr->env->GetFieldID(jobj->java_class, key.c_str(), "F")){
			cout << "[Error] Failed to link input \"" + key + "\" which does not exist in Java class \"" << full_class_name << endl;
			return;
		}
		input_keys[key] = p_float;
	#endif
}

void ComJava::Link_output_java(string key, float *p_float){
	Link_output(key, p_float);
	#ifdef ENABLE_JAVA
		if(!jstr->env->GetFieldID(jobj->java_class, key.c_str(), "F")){
			cout << "[Error] Failed to link output \"" + key + "\" which does not exist in Java class \"" << full_class_name << endl;
			return;
		}
		output_keys[key] = p_float;
	#endif
}

void ComJava::Send_to_java(){
	#ifdef ENABLE_JAVA
		for(PFloatMap::iterator it = input_keys.begin(); it != input_keys.end(); ++it){
			jfieldID jField = jstr->env->GetFieldID(jobj->java_class, it->first.c_str(), "F");
			jfloat jValue = (jfloat) *(it->second);
			jstr->env->SetFloatField(jobj->java_instance, jField, jValue);
		}
	#endif
}

void ComJava::Receive_from_java(){
	#ifdef ENABLE_JAVA
		for(PFloatMap::iterator it = output_keys.begin(); it != output_keys.end(); ++it){
			jfieldID jField = jstr->env->GetFieldID(jobj->java_class, it->first.c_str(), "F");
			jfloat jValue = jstr->env->GetFloatField(jobj->java_instance, jField);
			*(it->second) = (float) jValue;
		}
	#endif
}

void ComJava::Job_java(){
	#ifdef ENABLE_JAVA
		jstr->env->CallVoidMethod(jobj->java_instance, jobj->java_job);
	#endif	
}
