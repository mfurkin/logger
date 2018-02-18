/*
 * LoggingRecord.h
 *
 *  Created on: 22 дек. 2017 г.
 *
 */

#ifndef LOGGINGRECORD_H_
#define LOGGINGRECORD_H_
#include "Logger.h"
#include <time.h>

class Logger;

class LoggingRecord {
public:
	LoggingRecord(std::string& aTag, std::string& aMsg, time_t aTime, Logger* aLoggerPtr);
	virtual ~LoggingRecord();
	void writeLog();
private:
	std::string tag,msg;
	time_t logTime;
	Logger* logger_ptr;
};

#endif /* LOGGINGRECORD_H_ */
