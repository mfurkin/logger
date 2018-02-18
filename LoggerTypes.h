/*
 * LoggerTypes.h
 *
 *  Created on: 22 дек. 2017 г.
 *
 */

#ifndef LOGGERTYPES_H_
#define LOGGERTYPES_H_
typedef void  (*CreateLoggerProc) (const char* name, const char* fname);
typedef int (*LogProc) (const char* loggerName, const char* tag, const char* msg);
typedef int (*LogPtrProc) (const char* loggerName, const char* tag, const char* msg, unsigned ptr);
typedef void (*DeleteLoggerProc)  (const char* name);




#endif /* LOGGERTYPES_H_ */
