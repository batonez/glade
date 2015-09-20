#ifdef _WIN32
#include <windows.h>
#include <windows.h>
#include <DbgHelp.h>
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


void print_stack(void)
{
#ifdef _WIN32
  unsigned int   i;
  void         * stack[100];
  unsigned short frames;
  SYMBOL_INFO  * symbol;
  HANDLE         process;

  process = GetCurrentProcess();
  SymInitialize(process, NULL, true);
  frames               = CaptureStackBackTrace(0, 100, stack, NULL);
  symbol               = (SYMBOL_INFO *) calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
  symbol->MaxNameLen   = 255;
  symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

  for (i = 0; i < frames; i++)
  {
    SymFromAddr(process, (DWORD64) (stack[i]), 0, symbol);
    log("%i: %s - 0x%0X\n", frames - i - 1, symbol->Name, symbol->Address);
  }

  free(symbol);
#endif
}
