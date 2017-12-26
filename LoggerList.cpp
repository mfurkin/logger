/*
 * LoggerList.cpp
 *
 *  Created on: 22 дек. 2017 г.
 *      Author:
 */

#include "LoggerList.h"

LoggerList& LoggerList::getLoggerList() {
	static LoggerList list;
	return list;
}

void LoggerList::addLogger(std::string name, std::string fname) {
	EnterCriticalSection(&c_section);
	std::map<std::string,Logger*>::iterator it = loggers.find(name);
	if (!(exists(name))) {
		loggers[name] = new Logger(fname);
	}
	LeaveCriticalSection(&c_section);
}

void LoggerList::deleteLogger(std::string name) {
	EnterCriticalSection(&c_section);
	if (exists(name)) {
		Logger* ptr = loggers[name];
		(*ptr).stop();
		loggers.erase(name);
	}
	LeaveCriticalSection(&c_section);
}

Logger* LoggerList::getLogger(std::string name) {
	Logger* result = NULL;
	EnterCriticalSection(&c_section);
	result = (exists(name)) ? loggers[name] : NULL;
	LeaveCriticalSection(&c_section);
	return result;
}

LoggerList::LoggerList() {
	InitializeCriticalSection(&c_section);
}

LoggerList::~LoggerList() {
	DeleteCriticalSection(&c_section);
}

void LoggerList::stopLogger(std::string name) {
}

int LoggerList::exists(std::string& name) {
	std::map<std::string,Logger*>::iterator it = loggers.find(name);
	return (it == loggers.end());
}

void __declspec(dllexport) createLogger(const char* name, const char* fname) {
	LoggerList& list = LoggerList::getLoggerList();
	list.addLogger(std::string(name),std::string(fname));
}

int __declspec(dllexport) log(const char* loggerName, const char* tag, const char* msg) {
	int result;
	LoggerList& list = LoggerList::getLoggerList();
	Logger* ptr = list.getLogger(loggerName);
	if (!(ptr))
		result = 0;
	else {
		result = 1;
		std::string tag_st(tag),msg_st(msg);
		(*ptr).log(tag_st,msg_st);
	}
	return result;
}

int __declspec(dllexport) logPtr(const char* loggerName, const char* tag, const char* msg, unsigned p) {
	int result;
		LoggerList& list = LoggerList::getLoggerList();
		std::string loggerName_st(loggerName);
		Logger* ptr = list.getLogger(loggerName_st);
		if (!(ptr))
			result = 0;
		else {
			result = 1;
			std::string tag_st(tag),msg_st(msg);
			(*ptr).logPtr(tag_st,msg_st,p);
		}
		return result;
}

void __declspec(dllexport) deleteLogger(const char* name) {
	LoggerList& list = LoggerList::getLoggerList();
	std::string name_st(name);
	list.deleteLogger(name_st);
}
