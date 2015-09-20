#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#include "glade/os/windows-inc.h"
#endif

#include <gl/GL.h>
#include <gl/glext.h>

PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLRELEASESHADERCOMPILERPROC glReleaseShaderCompiler;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLUNIFORM4FPROC glUniform4f;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLDELETEPROGRAMPROC glDeleteProgram;

void loadFunctions(void) {
  glCreateProgram = (PFNGLCREATEPROGRAMPROC) wglGetProcAddress("glCreateProgram");
  glUseProgram = (PFNGLUSEPROGRAMPROC) wglGetProcAddress("glUseProgram");
  glAttachShader = (PFNGLATTACHSHADERPROC) wglGetProcAddress("glAttachShader");
  glLinkProgram = (PFNGLLINKPROGRAMPROC) wglGetProcAddress("glLinkProgram");
  glReleaseShaderCompiler = (PFNGLRELEASESHADERCOMPILERPROC) wglGetProcAddress("glReleaseShaderCompiler");
  glCreateShader = (PFNGLCREATESHADERPROC) wglGetProcAddress("glCreateShader");
  glShaderSource = (PFNGLSHADERSOURCEPROC) wglGetProcAddress("glShaderSource");
  glCompileShader = (PFNGLCOMPILESHADERPROC) wglGetProcAddress("glCompileShader");
  glGetShaderiv = (PFNGLGETSHADERIVPROC) wglGetProcAddress("glGetShaderiv");
  glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) wglGetProcAddress("glGetShaderInfoLog");
  glDeleteShader = (PFNGLDELETESHADERPROC) wglGetProcAddress("glDeleteShader");
  glGenBuffers = (PFNGLGENBUFFERSPROC) wglGetProcAddress("glGenBuffers");
  glBufferData = (PFNGLBUFFERDATAPROC) wglGetProcAddress("glBufferData");
  glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) wglGetProcAddress("glDeleteBuffers");
  glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) wglGetProcAddress("glUniformMatrix4fv");
  glUniform4f = (PFNGLUNIFORM4FPROC) wglGetProcAddress("glUniform4f");
  glUniform1i = (PFNGLUNIFORM1IPROC) wglGetProcAddress("glUniform1i");
  glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) wglGetProcAddress("glVertexAttribPointer");
  glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress("glEnableVertexAttribArray");
  glActiveTexture = (PFNGLACTIVETEXTUREPROC) wglGetProcAddress("glActiveTexture");
  glUniform1f = (PFNGLUNIFORM1FPROC) wglGetProcAddress("glUniform1f");
  glUniform3f = (PFNGLUNIFORM3FPROC) wglGetProcAddress("glUniform3f");
  glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress("glDisableVertexAttribArray");
  glBindBuffer = (PFNGLBINDBUFFERPROC) wglGetProcAddress("glBindBuffer");
  glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) wglGetProcAddress("glGetUniformLocation");
  glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) wglGetProcAddress("glGetAttribLocation");
  glGetProgramiv = (PFNGLGETPROGRAMIVPROC) wglGetProcAddress("glGetProgramiv");
  glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) wglGetProcAddress("glGetProgramInfoLog");
  glDeleteProgram = (PFNGLDELETEPROGRAMPROC) wglGetProcAddress("glDeleteProgram");
}