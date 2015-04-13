/*
 * @(#)		CoreJava.h
 * @version	1.0
 * @autor	C. Rouvière
 */

/**
 * Provide Java Virtual Machine
 */

#ifndef COREJAVA_H
#define COREJAVA_H

#ifdef ENABLE_JAVA
	#include <jni.h>
#endif

#include <iostream>

typedef struct{
	#ifdef ENABLE_JAVA
		JavaVM* jvm;
		JNIEnv* env;
	#endif
}COREJAVA;

class CoreJava{

public:

	CoreJava();
	~CoreJava();

	void Java_init();
	void Java_close();
	COREJAVA* Get_jstr();
	bool Is_connected();
	
private:

	bool connected;

	COREJAVA* jstr;
	COREJAVA* Build_jvm();

};

#endif
