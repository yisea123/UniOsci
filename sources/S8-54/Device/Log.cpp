#include "Log.h"
#include "Display/Display.h"
#include "Settings/Settings.h"
#include "VCP/VCP.h"
#include <stdarg.h>
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool loggerUSB = false;


#define SIZE_BUFFER_LOG 200

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Log_Write(TypeTrace type, char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    char *pointer = buffer;

    if (type == TypeTrace_Error)
    {
        buffer[0] = 0;
        strcat(buffer, "!!! ERROR !!! ");
        while (*pointer++) {};
        ++pointer;
    }
    va_list args;
    va_start(args, format);
    vsprintf(pointer, format, args);
    va_end(args);
    Display::AddStringToIndicating(buffer);
    if(loggerUSB)
    {
        VCP::SendFormatStringAsynch(buffer);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Log_Trace(TypeTrace type, const char *module, const char *func, int numLine, char *format, ...)
{
    char buffer[SIZE_BUFFER_LOG];
    char message[SIZE_BUFFER_LOG];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    const int SIZE = 20;
    char numBuffer[SIZE];
    snprintf(numBuffer, 100, ":%d", numLine);
    message[0] = 0;
    if (type == TypeTrace_Error)
    {
        strcat(message, "!!!ERROR!!! ");
    }
    else if (type == TypeTrace_Info)
    {
        strcat(message, "            ");
    }
    strcat(message, module);
    strcat(message, " ");
    strcat(message, func);
    strcat(message, numBuffer);
    Display::AddStringToIndicating(message);
    Display::AddStringToIndicating(buffer);
    if(loggerUSB)
    {
        VCP::SendFormatStringAsynch(message);
        VCP::SendFormatStringAsynch(buffer);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Log_DisconnectLoggerUSB(void)
{
    //static uint8 data = 20;
    //Log_Write("������� %d", data);
    //VCP_SendData(&data, 1);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Log_EnableLoggerUSB(bool enable)
{
    loggerUSB = enable;
}
