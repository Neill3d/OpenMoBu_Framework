#pragma once

#define LOG_TRACE(msg, ...) if (GetLogger()) GetLogger()->Trace(msg, __VA_ARGS__); 


class CLoggerCallback
{
public:
	virtual void Trace(const char* message, ...) = 0;
};

CLoggerCallback* GetLogger();
void SetLogger(CLoggerCallback* callback);
