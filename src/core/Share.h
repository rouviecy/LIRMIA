/*
 * @(#)		Share.h
 * @version	2.0
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

typedef std::map <std::string, float> FloatMap;
typedef std::map <std::string, float*> PFloatMap;
typedef std::vector <std::string> StringVec;

class Share{

public:

	Share();
	void Create_data(std::string key);
	void Send(PFloatMap input);
	FloatMap Receive(StringVec keys);

private:

	std::mutex mu;
	FloatMap data;

	void Lock();
	void Unlock();
};

#endif
