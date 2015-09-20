#pragma once

#include "Shader.h"

class DefaultShaderProgram : public ShaderProgram
{
  public:
    virtual void getUniformLocations()
    {
     
    }
    
    virtual void setUniformValues(Drawable *drawable)
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