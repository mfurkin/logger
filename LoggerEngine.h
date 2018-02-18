/*
 * LoggerEngine.h
 *
 *  Created on: 26 ���. 2017 �.
 *
 */

#ifndef LOGGERENGINE_H_
#define LOGGERENGINE_H_
#ifdef __cplusplus
#include <string>

class LoggerEngine {
public:
	inline LoggerEngine() {};
	inline virtual ~LoggerEngine() {};
	virtual void log(std::string& tag,  std::string msg) = 0;
	virtual void logPtr(std::string& tag, std::string msg, unsigned ptr) = 0;
	virtual void logString(std::string& tag, std::string msg, std::string& msg2) = 0;
};

#endif

#endif /* LOGGERENGINE_H_ */
