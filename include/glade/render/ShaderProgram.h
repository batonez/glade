#pragma once

#include "Shader.h"

class ShaderProgram
{
  public:
    Shader *vertexShader;
    Shader *fragmentShader;
    int     gpuHandle;

    virtual void getUniformLocations() = 0;
    virtual void setUniformValues() = 0;
    virtual void erase() = 0;

    bool hasGpuHandle()
    {
      return gpuHandle >= 0;
    }

  protected:
    ShaderProgram():
      vertexShader(NULL),
      fragmentShader(NULL),
      gpuHandle(-1)
    {
    }
};