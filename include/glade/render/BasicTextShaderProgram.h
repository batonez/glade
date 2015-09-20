#pragma once

#include "Shader.h"
#include "ShaderProgram.h"
#include "Material.h"
#include "DefaultShaderProgram.h"
#include "../math/Vector.h"
#include "../log/log.h"
#include "../exception/GladeException.h"
#include "glade/opengl/functions.h"
#include <fstream>

class BasicTextShaderProgram : public DefaultShaderProgram
{
  public:
    static BasicTextShaderProgram INSTANCE;
    
    Material material;
    Vector4f constantColor;
    bool replaceOriginalColor;
    bool lit;
    
  protected:
    Shader *vertexShader;
    Shader *fragmentShader;
    
    GLuint uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess,
			uReplaceColor, uColor, uLight, uLightDirection, uLightAmbient, uLightDiffuse, uLightSpecular, uLightHalfplane;
    
    // directional light (hardcoded)
    float lightDirection[3], lightAmbient[4], lightDiffuse[4], lightSpecular[4];
  
    BasicTextShaderProgram():
      DefaultShaderProgram(),
      vertexShader(NULL),
      fragmentShader(NULL),
      replaceOriginalColor(false),
      lit(false)
    {
      std::ifstream vertexInput("D:/Codespace/Gladen/res/shaders/gl/vertex.glsl");
      std::ifstream fragmentInput("D:/Codespace/Gladen/res/shaders/gl/fragment.glsl");
      
      if (!fragmentInput.good() || !vertexInput.good()) {
        throw GladeException("Shader input is broken");
      }
      
      vertexShader = new Shader(vertexInput);
      fragmentShader = new Shader(fragmentInput);
      
      // directional light (hardcoded)
      lightDirection[0] = -1.0f; lightDirection[1]= 0.0f; lightDirection[2] = 1.0f;
      lightAmbient[0] = 0.5f; lightAmbient[1] = 0.5f; lightAmbient[2] = 0.5f; lightAmbient[3] = 0.5f; lightAmbient[4] = 1.0f;
      lightDiffuse[0] = 0.5f; lightDiffuse[1] = 0.5f; lightDiffuse[2] = 0.5f; lightDiffuse[3] = 1.0f;
      lightSpecular[0] = 1.0f; lightSpecular[1] = 1.0f; lightSpecular[2] = 1.0f; lightSpecular[3] = 1.0f;
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
      uReplaceColor		= glGetUniformLocation(gpuHandle, "uReplaceColor");
      uLight 				= glGetUniformLocation(gpuHandle, "uLight");
      uLightDirection 	= glGetUniformLocation(gpuHandle, "uLightDirection");
      uLightHalfplane		= glGetUniformLocation(gpuHandle, "uLightHalfplane");
      uLightAmbient 		= glGetUniformLocation(gpuHandle, "uLightAmbient");
      uLightDiffuse 		= glGetUniformLocation(gpuHandle, "uLightDiffuse");
      uLightSpecular 		= glGetUniformLocation(gpuHandle, "uLightSpecular");
      uMaterialAmbient 	= glGetUniformLocation(gpuHandle, "uMaterialAmbient");
      uMaterialDiffuse 	= glGetUniformLocation(gpuHandle, "uMaterialDiffuse");
      uMaterialSpecular 	= glGetUniformLocation(gpuHandle, "uMaterialSpecular");
      uMaterialShininess 	= glGetUniformLocation(gpuHandle, "uMaterialShininess");
    }

    virtual void setUniformValues()
    {
      DefaultShaderProgram::setUniformValues();
      
      glUniform4f(uColor, constantColor.x, constantColor.y, constantColor.z, constantColor.w);
      
      glUniform1i(uReplaceColor, replaceOriginalColor ? 1 : 0);
      
      if (lit) {
        //halfplane = normalize(dir - look) // <- Dynamic
        float halfplaneXZ = (float)(-1 / sqrt(2.0));
        glUniform3f(uLightHalfplane, halfplaneXZ, 0, halfplaneXZ);
        
        glUniform4f(uMaterialAmbient, material.ambient[0], material.ambient[1], material.ambient[2], material.ambient[3]);
        glUniform4f(uMaterialDiffuse, material.diffuse[0], material.diffuse[1], material.diffuse[2], material.diffuse[3]);
        glUniform4f(uMaterialSpecular, material.specular[0], material.specular[1], material.specular[2], material.specular[3]);
        glUniform1f(uMaterialShininess, material.shininess);
      
        glUniform3f(uLightDirection, lightDirection[0], lightDirection[1], lightDirection[2]);
        glUniform4f(uLightAmbient, lightAmbient[0], lightAmbient[1], lightAmbient[2], lightAmbient[3]);
        glUniform4f(uLightDiffuse, lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
        glUniform4f(uLightSpecular, lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
      
        glUniform1i(uLight, 1);
      } else {
        glUniform1i(uLight, 0);
      }
    }

    virtual void erase()
    {
      DefaultShaderProgram::erase();
    }
};