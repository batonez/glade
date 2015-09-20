#pragma once

#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class FreetypeFont: public Font
{
  private:
    static unsigned int instances;
    
    std::vector<unsigned char> faceBuffer;
    float minViewportDimension;
    FT_Face face;
    
    float stringScaleX, stringScaleY;
    
  public:  
    FreetypeFont(Path &face_file_path, int viewport_width, int viewport_height);
    ~FreetypeFont();
    
    virtual void setFontSizePixels(unsigned int width, unsigned int height = 0);
    virtual GladeObject::Drawables *createDrawablesForString(const std::string &string);
};
