#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#ifdef ANDROID
#include <android/log.h>
#endif // ANDROID

#include <stdio.h>
#include <stdarg.h>

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
#elif ANDROID
    __android_log_print(ANDROID_LOG_INFO, "glade", "%s", buf);
#else
#error "Glade logging is not implemented for this platform"
#endif

}