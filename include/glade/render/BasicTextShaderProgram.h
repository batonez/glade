#pragma once

#include "Shader.h"
#include "ShaderProgram.h"

class BasicTextShaderProgram : public ShaderProgram
{
  public:
    BasicTextShaderProgram(): ShaderProgram()
    {
      // create shaders from known sources
    }

    virtual void getUniformLocations()
    {
    }

    virtual void setUniformValues()
    {
    }

    virtual void erase()
    {
    }
};