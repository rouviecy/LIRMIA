#include "CoreJava.h"

using namespace std;

CoreJava::CoreJava(){
	connected = false;
}

CoreJava::~CoreJava(){
	Java_close();
}

COREJAVA* (CoreJava::Build_jvm()){

	#ifdef ENABLE_JAVA

		// Tmp pointers
		JavaVM* jvm;
		JNIEnv* env;

		// JAR path
		JavaVMOption options;
		options.optionString = const_cast<char*>("-Djava.class.path=./Java_parts.jar");

		// VM parameters
		JavaVMInitArgs vm_args;
		vm_args.version = JNI_VERSION_1_6;
		vm_args.nOptions = 1;
		vm_args.options = &options;
		vm_args.ignoreUnrecognized = false;

		// Create virtual machine and initialize environment
		JNI_GetDefaultJavaVMInitArgs(&vm_args);
		int ret = JNI_CreateJavaVM(&jvm, (void**) &env, &vm_args);
		if(ret < 0){
			return NULL;
		}
		jvm->AttachCurrentThread((void**) &env, NULL);

		// Return structure
		COREJAVA* result = new COREJAVA();
		result->jvm = jvm;
		result->env = env;
		return result;

	#else
		return NULL;
	#endif

}

void CoreJava::Java_init(){

	if(connected){
		cout << "[Warning] java is already connected !" << endl;
		return;
	}

	#ifdef ENABLE_JAVA
		jstr = Build_jvm();
		if(!jstr){
			cout << "[Error] Failed to launch Java VM" << endl;
			return;
		}
		connected = true;
	#else
		cout << "[Warning] You are trying to connect Java without Java enabled (see CMakeLists.txt)" << endl;
	#endif
}

void CoreJava::Java_close(){
	#ifdef ENABLE_JAVA
		if(connected){
			delete jstr;
			connected = false;
		}
	#endif
}

bool CoreJava::Is_connected(){return connected;}
COREJAVA* (CoreJava::Get_jstr()){return jstr;}
