/*
 * LoggingRecord.cpp
 *
 *  Created on: 22 дек. 2017 г.
 *      Author: Алёна
 */

#include "LoggingRecord.h"


LoggingRecord::LoggingRecord(std::string& aTag, std::string& aMsg, time_t aLogTime, Logger* aLoggerPtr):tag(aTag),msg(aMsg),logTime(aLogTime),
							 logger_ptr(aLoggerPtr) {
}

LoggingRecord::~LoggingRecord() {
}

void LoggingRecord::writeLog() {
	(*logger_ptr).writeLog(tag,msg,logTime);
}
