#pragma once

#include <vector>
#include <memory>

#include <glade/render/Drawable.h>
#include <glade/render/Texture.h>
#include <glade/GladeObject.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H
#include FT_GLYPH_H
#include FT_BBOX_H

class FreetypeFont
{
  private:
    std::vector<unsigned char>     faceBuffer;
    std::shared_ptr<ShaderProgram> shaderProgram;
    float minViewportDimension;
    
    static unsigned int instances;
    
    FT_Face     face;
    
    float stringScaleX, stringScaleY;
    std::vector<FT_Glyph>  glyphs;
    std::vector<FT_Vector> pos;
    int verticalCorrection;
    
  public:  
    FreetypeFont(Path &face_file_path, int viewport_width, int viewport_height);
    ~FreetypeFont();
    
    void setFontSizePixels(unsigned int width, unsigned int height = 0);
    void setShaderProgram(std::shared_ptr<ShaderProgram> &shader_program) {
      shaderProgram = shader_program;
    }
    
    GladeObject::Drawables *createDrawablesForString(const std::string &string);
    float getLastStringScaleX() { return stringScaleX; }
    float getLastStringScaleY() { return stringScaleY; }
    
  private:
    void measureString(const std::string &string);
    void computeStringBbox(FT_BBox *bbox);
    Drawable *createDrawableFromBitmap(FT_Bitmap *bitmap);
    Drawable *createDrawableForUTF32Char(FT_ULong charcode);
    
    void checkFreeTypeError();
};
