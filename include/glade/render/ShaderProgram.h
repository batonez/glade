#pragma once

#include "Shader.h"

class ShaderProgram
{
  public:
    int     gpuHandle;

    virtual void getUniformLocations() = 0;
    virtual void setUniformValues() = 0;
    virtual Shader* getVertexShader() = 0;
    virtual Shader* getFragmentShader() = 0;
    virtual void erase() = 0;

    bool hasGpuHandle()
    {
      return gpuHandle >= 0;
    }

  protected:
    ShaderProgram():
      gpuHandle(-1)
    {
    }
};