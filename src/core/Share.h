/*
 * @(#)		Share.h
 * @version	2.1
 * @autor	C. Rouvière
 */

/**
 * Workspace to share data
 */

#ifndef SHARE
#define SHARE

#include <map>
#include <mutex>
#include <vector>
#include <iostream>

enum T_DATA { COMBOOL, COMINT, COMFLOAT };

typedef struct{
	T_DATA data_type;
	int size;
	void *p_data;
}DATA;

typedef std::map <std::string, void*> PVoidMap;
typedef std::map <std::string, DATA> PDataMap;
typedef std::vector <std::string> StringVec;

class Share{

public:

	Share();
	~Share();

	bool Create_data(std::string key, T_DATA data_type, int size);
	void Update(PVoidMap io_data, bool is_input);

	static std::string Convert_type_str(T_DATA data_type, int size);

private:

	std::mutex mu;
	PDataMap data;

	void Lock();
	void Unlock();

};

#endif
