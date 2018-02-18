/*
 * LoggerList.h
 *
 *  Created on: 22 дек. 2017 г.
 *
 */

#ifndef LOGGERLIST_H_
#define LOGGERLIST_H_
#include <map>
#include "Logger.h"

class LoggerList {
public:
	static LoggerList& getLoggerList();
	virtual ~LoggerList();
	void addLogger(std::string name, std::string fname);
	void deleteLogger(std::string name);
	Logger* getLogger(std::string name);
	void stopLogger(std::string name);
private:
	LoggerList();
	int exists(std::string& name);
	LoggerList(const LoggerList&) = delete;
	LoggerList& operator = (LoggerList& aList) = delete;
	std::map<std::string,Logger*> loggers;
	CRITICAL_SECTION c_section;
};
/*
#ifdef __cplusplus__
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif
*/
extern "C" void __declspec(dllexport) createLogger(const char* name, const char* fname);
extern "C" int __declspec(dllexport) log(const char* loggerName, const char* tag, const char* msg);
extern "C" int __declspec(dllexport) logPtr(const char* loggerName, const char* tag, const char* msg, unsigned ptr);
extern "C" void __declspec(dllexport)  deleteLogger(const char* name);
#endif /* LOGGERLIST_H_ */
