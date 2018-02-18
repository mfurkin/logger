/*
 * Logger.h
 *
 *  Created on: 22 дек. 2017 г.
 *
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <process.h>
#include <queue>
#include <string>

#include <wtypes.h>
#include <winbase.h>
#include <windef.h>
#include <windows.h>
#include "LoggingRecord.h"
enum {END_FLAG_INDEX = 1};
class LoggingRecord;

class Logger {
public:
	Logger(std::string& aFname);
	virtual ~Logger();
	void log(std::string& tag, std::string&  msg);
	void logPtr(std::string& tag, std::string& msg, unsigned ptr);
	void writeLog(std::string& tag, std::string& msg, time_t logTime);
	void stop();
	static unsigned WINAPI loggingThread(void* p);
	void writeToFile(HANDLE file,unsigned char* buf, unsigned len);
private:
	HANDLE createFile();
	HANDLE openFile();
	HANDLE getHandle(unsigned disposition);
	LoggingRecord* getFirst();
	void removeFirst();
	unsigned logThread();
	std::string fname;
	CRITICAL_SECTION c_section;
	HANDLE request;//,file;
	std::queue<LoggingRecord*> log_queue;
	int in_progress;
	HANDLE endFlag;
//	,threadCompleted;
};

#endif /* LOGGER_H_ */
