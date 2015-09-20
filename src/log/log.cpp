#if _WIN32
#include "glade/os/windows-inc.h"
#endif // _WIN32

#include <stdio.h>

#include "glade/log/log.h"

void log(const char* format, ...)
{

    char buf[1024];
    
    va_list args;
    va_start(args, format);
    
    vsprintf(buf, format, args);
    
    va_end(args);
    
#ifdef _WIN32    
    OutputDebugString(buf);
#endif
}