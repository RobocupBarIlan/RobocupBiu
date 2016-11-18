#ifndef _LOG_H
#define _LOG_H

#include "Includes.h"
#include "opencv2/flann/logger.h"

class Log
{
public:
	// Singletone
	static Log* GetInstance();
	~Log();

	void Info(string message, string category);
	void Warning(string message, string category);
	void Error(string message, string category);
	void Fatal(string message, string category);

private:
	Log();
	static Log* m_instance;
	string GetCurrentTimestampString(bool addMilliseconds = true);
	string FormatMessage(string message, string category);
};

#endif
