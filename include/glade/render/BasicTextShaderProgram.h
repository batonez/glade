#pragma once

#include <fstream>

#include "Shader.h"
#include "ShaderProgram.h"
#include "Material.h"
#include "DefaultShaderProgram.h"
#include "../math/Vector.h"
#include "../log/log.h"
#include "../exception/GladeException.h"
#include "glade/opengl/functions.h"
#include "Drawable.h"

class BasicTextShaderProgram : public DefaultShaderProgram
{
  public:
    static BasicTextShaderProgram INSTANCE;
    
  protected:
    Shader *vertexShader;
    Shader *fragmentShader;
    
    GLuint uReplaceColor, uColor;
    
    // directional light (hardcoded)
    float lightDirection[3], lightAmbient[4], lightDiffuse[4], lightSpecular[4];
  
    BasicTextShaderProgram():
      DefaultShaderProgram(),
      vertexShader(NULL),
      fragmentShader(NULL)
    {
      std::ifstream vertexInput("D:/Codespace/Gladen/res/shaders/gl/vertex_text.glsl");
      std::ifstream fragmentInput("D:/Codespace/Gladen/res/shaders/gl/fragment_text.glsl");
      
      if (!fragmentInput.good() || !vertexInput.good()) {
        throw GladeException("Shader input is broken");
      }
      
      vertexShader = new Shader(vertexInput);
      fragmentShader = new Shader(fragmentInput);
    }
    
  public:
    ~BasicTextShaderProgram()
    {
      if (vertexShader) {
        delete vertexShader;
      }
      
      if (fragmentShader) {
        delete fragmentShader;
      }
    }
  
    virtual Shader* getVertexShader()
    {
      return vertexShader;
    }
    
    virtual Shader* getFragmentShader()
    {
      return fragmentShader;
    }
  
    virtual void getUniformLocations()
    {
      DefaultShaderProgram::getUniformLocations();
      
    	uColor				= glGetUniformLocation(gpuHandle, "uColor");
      uReplaceColor	= glGetUniformLocation(gpuHandle, "uReplaceColor");
    }

    virtual void setUniformValues(Drawable *drawable)
    {
      DefaultShaderProgram::setUniformValues(drawable);
      
      Vector4f constantColor = drawable->shaderVec4Uniforms["constantColor"];
      bool replaceOriginalColor = drawable->shaderBoolUniforms["replaceOriginalColor"];
      
      glUniform4f(uColor, constantColor.x, constantColor.y, constantColor.z, constantColor.w);
      glUniform1i(uReplaceColor, replaceOriginalColor ? 1 : 0);
    }

    virtual void erase()
    {
      DefaultShaderProgram::erase();
    }
};