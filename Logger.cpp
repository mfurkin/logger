/*
 * Logger.cpp
 *
 *  Created on: 22 дек. 2017 г.
 *
 */

#include "Logger.h"

Logger::Logger(std::string& aFname):fname(aFname) {
	InitializeCriticalSection(&c_section);
	request = CreateEvent(NULL,FALSE,FALSE,NULL);
	HANDLE file = createFile();
	CloseHandle(file);
	endFlag = CreateEvent(NULL,FALSE,FALSE,NULL);
//	threadCompleted = CreateEvent(NULL,FALSE,FALSE,NULL);
	in_progress = 1;
	_beginthreadex(NULL,0,loggingThread,this,0,NULL);
}

Logger::~Logger() {
	printf("Logger dtor\n");
	CloseHandle(request);
	CloseHandle(endFlag);
//	CloseHandle(threadCompleted);
	DeleteCriticalSection(&c_section);
}

void Logger::log(std::string& tag, std::string& msg) {
	printf("LOgger::Log enter tag=%s, msg=%s\n",tag.c_str(),msg.c_str());
	EnterCriticalSection(&c_section);
	time_t cur_time = time(NULL);
	log_queue.push(new LoggingRecord(tag,msg,cur_time,this));
	SetEvent(request);
	LeaveCriticalSection(&c_section);
}

void Logger::logPtr(std::string& tag, std::string& msg, unsigned ptr) {
	char st[MAX_PATH];
	sprintf(st,"%s %d",msg.c_str(),ptr);
	std::string msg_st = std::string(st);
	log(tag,msg_st);
}

void Logger::writeLog(std::string& tag, std::string& msg, time_t logTime) {
	printf("Logger::writeLog enter tag=%s msg=%s time=%ld\n",tag.c_str(),msg.c_str(),logTime);
	char st[MAX_PATH];
	HANDLE file;
	time_t curTime = logTime;
	struct tm* tm_ptr = localtime(&curTime);
	unsigned short month, year;
	month = (*tm_ptr).tm_mon + 1;
	year = (*tm_ptr).tm_year + 1900;
	sprintf(st,"%02d:%02d:%04d %02d:%02d^%02d %s %s\n",(*tm_ptr).tm_mday,month,year,(*tm_ptr).tm_hour,(*tm_ptr).tm_min,(*tm_ptr).tm_sec,
			tag.c_str(),msg.c_str());
	file = openFile();
	writeToFile(file,(unsigned char*)st,strlen(st)*sizeof(char));
}

unsigned WINAPI Logger::loggingThread(void* p) {
	Logger* ptr = (Logger*) p;
	return (*ptr).logThread();
}

HANDLE Logger::createFile() {
	return getHandle(CREATE_ALWAYS);
}

HANDLE Logger::openFile() {
	return getHandle(OPEN_EXISTING);
}

HANDLE Logger::getHandle(unsigned disposition) {
//	return CreateFile(fname.c_str(),FILE_APPEND_DATA,0,NULL,disposition,FILE_FLAG_OVERLAPPED,NULL);
	return CreateFile(fname.c_str(),FILE_APPEND_DATA,0,NULL,disposition,0,NULL);
}

void Logger::writeToFile(HANDLE file, unsigned char* buf, unsigned len) {
	static OVERLAPPED ovl;
	unsigned long bytesWritten;
//	HANDLE events[2]={file,endFlag};
	printf("Logger::writeToFile enter\n");
	ovl.Offset = -1;
	ovl.OffsetHigh = -1;
//	WriteFileEx(file,buf,len,&ovl,NULL);
	WriteFile(file,buf,len,&bytesWritten,&ovl);
	printf("Logger::writeToFile pt1\n");
	/*
	unsigned result = WaitForMultipleObjectsEx(2,events,FALSE,INFINITE,TRUE),index;
	index = result - WAIT_OBJECT_0;
	*/
	WaitForSingleObjectEx(file,INFINITE,TRUE);
	printf("Logger::writeToFile pt2 \n");
//	printf("Logger::writeToFile pt2 index=%d\n",index);
	/*
	if (index == END_FLAG_INDEX)
		CancelIo(file);
		*/
	printf("Logger::writeToFile pt3\n");
	CloseHandle(file);
	printf("Logger::writeToFile exit\n");
}

void Logger::stop() {

	in_progress = 0;
//	printf("Logger::stop endFlag=%p threadCompleted=%p\n",endFlag,threadCompleted);
//	printf("Logger::stop endFlag=%p \n",endFlag);
	SetEvent(endFlag);
//	printf("Logger:stop pt1\n");
//	WaitForSingleObjectEx(threadCompleted,INFINITE,TRUE);
//	SignalObjectAndWait(endFlag,threadCompleted,INFINITE,TRUE);
//	printf("Logger:stop  exit\n");
}

LoggingRecord* Logger::getFirst() {
	LoggingRecord* result;
	EnterCriticalSection(&c_section);
	result = log_queue.front();
	LeaveCriticalSection(&c_section);
	return result;
}

void Logger::removeFirst() {
	EnterCriticalSection(&c_section);
	log_queue.pop();
	LeaveCriticalSection(&c_section);
}

unsigned Logger::logThread() {
	unsigned result;
	HANDLE events[2] =  {request,endFlag};
	LoggingRecord* ptr;
	printf("logThread enter\n");
	for(;in_progress;) {
		result = WaitForMultipleObjectsEx(2,events,FALSE,INFINITE,TRUE);
		printf("logThread after WaitForMultipleObjectsEx result=%d\n",result);
		if (result == END_FLAG_INDEX)
			in_progress = 0;
		else
			if (!(result)) {
				printf("logThread pt1\n");
				for (;(!(log_queue.empty()));) {
					printf("logThread pt2\n");
					ptr = getFirst();
					printf("logThread pt3 ptr=%p\n",ptr);
					(*ptr).writeLog();
					removeFirst();
				}

			}
	}
	printf("logThread before SetEvent\n");
//	SetEvent(threadCompleted);
	printf("logThread exit\n");
	return 0;
}
