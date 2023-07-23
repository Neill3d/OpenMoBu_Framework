#include "Logger.h"

CLoggerCallback* g_Logger = nullptr;

CLoggerCallback* GetLogger()
{
    return g_Logger;
}

void SetLogger(CLoggerCallback* callback)
{
    g_Logger = callback;
}
