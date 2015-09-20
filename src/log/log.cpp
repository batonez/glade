#if _WIN32
#include <windows.h>
#endif // _WIN32

#include <stdio.h>

#include "glade/log/log.h"

void log(const char* format, ...)
{
    static char buf[5000]; // OMG fix this
    
    va_list args;
    va_start(args, format);
    
    vsprintf(buf, format, args);
    
    va_end(args);
    
#ifdef _WIN32    
    OutputDebugString(buf);
#endif
}