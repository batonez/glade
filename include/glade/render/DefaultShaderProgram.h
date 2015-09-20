#pragma once

#include "Shader.h"

class DefaultShaderProgram : public ShaderProgram
{
  public:
    virtual void getUniformLocations()
    {
     
    }
    
    virtual void setUniformValues()
    {
    }
    
    virtual void erase()
    {
    }
  
  protected:
    DefaultShaderProgram() : ShaderProgram()
    {
    }
};