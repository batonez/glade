#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <errno.h>
#include <execinfo.h>

#define MAX_STACK_FRAMES 64
extern char *program_invocation_name;

#include <glade/debug/log.h>

static char buf[5000]; // FIXME buffer overflow

static inline void debugStringOut() {
  printf("%s", buf);
}

void logn(const char *format, ...) {
  va_list args;
  va_start(args, format);

  vsprintf(buf, format, args);

  va_end(args);

  debugStringOut();
}

void log(const char *format, ...) {
  std::string formatWithNewLine(format);
  formatWithNewLine.append("\r\n");

  va_list args;
  va_start(args, format);

  vsprintf(buf, formatWithNewLine.c_str(), args);

  va_end(args);

  debugStringOut();
}

/* Resolve symbol name and source location given the path to the executable
   and an address */
int _addr2line(char const *const program_name, void const *const addr) {
  char addr2line_cmd[512] = {0};

  /* have addr2line map the address to the relent line in the code */
  sprintf(addr2line_cmd, "addr2line -f -p -e %.256s %p", program_name, addr);

  /* This will print a nicely formatted string specifying the
     function and source line of the address */
  return system(addr2line_cmd);
}

void backtrace(void) {
  void *stack_traces[MAX_STACK_FRAMES];
  int i, trace_size = 0;
  char **messages = (char **) NULL;

  trace_size = backtrace(stack_traces, MAX_STACK_FRAMES);
  messages = backtrace_symbols(stack_traces, trace_size);

  /* skip the first couple stack frames (as they are this function and
     our handler) and also skip the last frame as it's (always?) junk. */
  // for (i = 3; i < (trace_size - 1); ++i)
  // we'll use this for now so you can see what's going on
  for (i = 0; i < trace_size; ++i) {
    if (_addr2line(program_invocation_name, stack_traces[i]) != 0) {
      printf("  error determining line # for: %s\n", messages[i]);
    }

  }
  if (messages) { free(messages); }
}