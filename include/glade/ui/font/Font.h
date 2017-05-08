#pragma once

#include <glade/GladeObject.h>

class ShaderProgram;

// this is a stub font, that returns empty Drawables. Children of this class should implement the virtual methods
class Font
{
  protected:
    std::shared_ptr<ShaderProgram> shaderProgram;
    float stringScaleX, stringScaleY;
    
  public:
    Font(): stringScaleX(0), stringScaleY(0) {}
    Font(std::unique_ptr<std::vector<unsigned char> > &face_buffer,
      int viewport_width, int viewport_height) {}
    
    virtual void setFontSizePixels(unsigned int width, unsigned int height = 0) {}
    virtual void setFontSize(float fontSize) {}
    virtual float getFontSize() const { return 0; }
    virtual GladeObject::Drawables *createDrawablesForString(const std::string &string) { return new GladeObject::Drawables(); }

    void setShaderProgram(std::shared_ptr<ShaderProgram> &shader_program)
    {
      shaderProgram = shader_program;
    }
    
    float getLastStringScaleX() { return stringScaleX; }
    float getLastStringScaleY() { return stringScaleY; }
};
