#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string>

#ifdef _WIN32
#include <windows.h>
#include <DbgHelp.h>
#endif // _WIN32

#ifdef ANDROID
#include <android/log.h>
#endif // ANDROID

#include "glade/debug/log.h"

static char buf[5000]; // FIXME buffer overflow

static inline void debugStringOut()
{
#ifdef _WIN32    
  ::OutputDebugString(buf);
#elif ANDROID
  __android_log_print(ANDROID_LOG_INFO, "glade", "%s", buf);
#else
#error "Glade logging is not implemented for this platform"
#endif // _WIN32
}

void logn(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  
  vsprintf(buf, format, args);
  
  va_end(args);
    
  debugStringOut();
}

void log(const char* format, ...)
{
  std::string formatWithNewLine(format);
  formatWithNewLine.append("\r\n");
  
  va_list args;
  va_start(args, format);
  
  vsprintf(buf, formatWithNewLine.c_str(), args);
  
  va_end(args);
    
  debugStringOut();
}

void backtrace(void)
{
#if _WIN32
   unsigned int   i;
   void          *stack[100];
   unsigned short frames;
   SYMBOL_INFO   *symbol;
   HANDLE         process;

   process = ::GetCurrentProcess();
   ::SymInitialize(process, NULL, TRUE);
   frames = ::CaptureStackBackTrace(0, 100, stack, NULL);
   symbol = (SYMBOL_INFO*) calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
   symbol->MaxNameLen   = 255;
   symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

   for (i = 0; i < frames; i++) {
     ::SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
     log("%i: %s - 0x%0X\n", frames - i - 1, symbol->Name, symbol->Address);
   }

   free(symbol);
#else
#warning "Glade log stack trace is not implemented for this platform"
#endif // _WIN32
}
