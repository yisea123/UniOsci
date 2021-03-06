#pragma once   
#include "defines.h"

#define ASSERT_RET(cond, ...)
#define LOG_ERROR_TRACE(cond, ...)

#ifdef WIN32

void LOG_WRITE(char *);
void LOG_WRITE(char *, char *);
void LOG_WRITE(char *, const char *);
void LOG_WRITE(char *, char *, float);
void LOG_WRITE(char *, uint16);
void LOG_WRITE(char *, char *, uint16, uint16, uint16);
void LOG_WRITE(char *, DataSettings *, void *, void *, int);
void LOG_WRITE(char *, const char *, int, int, int);
void LOG_WRITE(char *, char *, char *, const char *);
#define LOG_ERROR(...)
#define LOG_FUNC_ENTER
#define LOG_FUNC_LEAVE
#define LOG_TRACE

#else

#define LOG_WRITE(...)  Log_Write(__VA_ARGS__)
#define LOG_ERROR(...)  Log_Error(__MODULE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_FUNC_ENTER  Log_Write("%s enter", __FUNCTION__);
#define LOG_FUNC_LEAVE  Log_Write("%s leave", __FUNCTION__);
#define LOG_TRACE       Log_Write("%s : %d", __MODULE__, __LINE__);

#endif

void Log_Error(const char *module, const char *func, int numLine, char *format, ...);
void Log_Write(char *format, ...);
void Log_DisconnectLoggerUSB();
void Log_EnableLoggerUSB(bool enable);
