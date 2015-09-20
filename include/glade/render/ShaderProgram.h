#pragma once

// remove this (see comment below)
#ifndef ANDROID
#include "glade/opengl/functions.h"
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

class Drawable;

class ShaderProgram
{
  public:
    int     gpuHandle;
    
    std::vector<char> vertexShaderSource;
    std::vector<char> fragmentShaderSource;

    ShaderProgram():
      gpuHandle(-1)
    {
    }
    
    unsigned getUniformLocation(const std::string &uniform_name)
    {
    // FIXME maybe move to renderer (here leave a map of string->unsigned)
    // in the renderer fill up this map at the time the drawable is added
      return glGetUniformLocation(gpuHandle, uniform_name.c_str()); 
    }
    
    void erase()
    {
      vertexShaderSource.clear();
      fragmentShaderSource.clear();
    }

    bool hasGpuHandle()
    {
      return gpuHandle >= 0;
    }
};