#pragma once

#include "Shader.h"
#include "glade/exception/GladeException.h"

class ShaderProgram
{
  private:
    Shader *vertexShader;
    Shader *fragmentShader;
    int     gpuHandle;
    
  public:
    ShaderProgram(Shader &vertexShader, Shader &fragmentShader):
      vertexShader(&vertexShader),
      fragmentShader(&fragmentShader),
      gpuHandle(-1)
    {    
    }
    
    virtual void getUniformLocations() = 0;
    virtual void setUniformValues() = 0;
};