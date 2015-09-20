#pragma once

#include <string>
#include <algorithm>
#include <memory>

#include "glade/render/Texture.h"
#include "glade/util/CSVReader.h"
#include "glade/util/ResourceManager.h"
#include "glade/util/Path.h"
#include "glade/GladeObject.h"
#include "glade/render/meshes/Rectangle.h"
#include "glade/render/Drawable.h"

class BitmapFont
{
  private:
    std::shared_ptr<Texture> atlas;
    std::shared_ptr<ShaderProgram> shaderProgram;
    int cellWidth, cellHeight;
    int firstGlyphAsciiCode;
    int glyphHeight;

     // Desired result font height in screen coordinates
    float fontSize;
    
  public:
    //FIXME should be private
    std::vector<int> glyphWidths;
    int numberOfGlyphsInARow;
    
    BitmapFont(std::shared_ptr<Texture> &atlas, int cell_width, int cell_height):
      fontSize(1),
      atlas(atlas),
      cellWidth(cell_width),
      cellHeight(cell_height)
    {
    }

    void setShaderProgram(std::shared_ptr<ShaderProgram> &shader_program)
    {
      shaderProgram = shader_program;
    }
    
    unsigned getGlyphIndexForAsciiCode(unsigned char asciiCode) const
    {
      unsigned glyphIndex = asciiCode - firstGlyphAsciiCode;
      
      if (glyphIndex < 0 || glyphIndex >= glyphWidths.size()) {
        glyphIndex = glyphWidths.size() - 1;
      }
      
      return glyphIndex;
    }
    
    Vector2i getGlyphPositionForIndex(int glyphIndex) const
    {
      Vector2i position;
      
      if (glyphIndex < 0 || glyphIndex >= glyphWidths.size()) {
        glyphIndex = glyphWidths.size() - 1;
      }
      
      position.x = glyphIndex % numberOfGlyphsInARow;
      position.y = glyphIndex / numberOfGlyphsInARow;
      
      return position;
    }
    
    // Should be manually freed after use
    GladeObject::Drawables* createDrawablesForString(const std::string &string)
    {
      GladeObject::Drawables* text = new GladeObject::Drawables(); 
      Drawable* rectangle = NULL;
      int glyphWidth;
      Vector2i glyphPosition;
      float nextOffsetX = 0;
      float halfStringWidth = getStringWidth(string);

      for (size_t i = 0; i < string.length(); ++i) {
        glyphPosition = getGlyphPositionForIndex(getGlyphIndexForAsciiCode(string[i]));
        glyphWidth = getGlyphWidth(string[i]);
        
        rectangle = new Drawable(Rectangle::INSTANCE, shaderProgram);
        rectangle->getTransform()->getScale()->x = ((float) glyphWidth / (float) glyphHeight) * fontSize;
        rectangle->getTransform()->getScale()->y = fontSize;
        
        nextOffsetX += rectangle->getTransform()->getScale()->x;
        rectangle->getTransform()->getPosition()->x = nextOffsetX - halfStringWidth;
        nextOffsetX += rectangle->getTransform()->getScale()->x;

        rectangle->setTexture(atlas);
//      rectangle->setConstantColor((float)(rand() % 100)/100.0f, (float)(rand() % 100)/100.0f, (float)(rand() % 100)/100.0f, 0.5f);
        rectangle->getTextureTransform()->setCurrentFrameNumber(glyphPosition.x);
        rectangle->getTextureTransform()->setCurrentAnimationNumber(glyphPosition.y);
        rectangle->getTextureTransform()->setCurrentFrameWidth(glyphWidth);
        rectangle->getTextureTransform()->setCurrentFrameHeight(glyphHeight);
        
        text->push_back(rectangle);
      }

      return text;
    }
    
    float getStringWidth(const std::string &string) const
    {
      float stringWidth = 0;
      
      for (size_t i = 0; i < string.length(); ++i) {      
        stringWidth += ((float) glyphWidths[string[i]] / (float) glyphHeight) * fontSize;
      }

      return stringWidth;
    }
      
    std::shared_ptr<Texture> getAtlas()
    {
      return atlas;
    }
    
    int getCellWidth() const
    {
      return cellWidth;
    }
    
    int getCellHeight() const
    {
      return cellHeight;
    }
    
    int getGlyphWidth(unsigned char asciiCode) const
    {
      return glyphWidths[asciiCode];
    }
    
    int getGlyphHeight() const
    {
      return glyphHeight;
    }
    
    int getNumberOfChars() const
    {
      return glyphWidths.size();
    }
    
    void setFontSize(float fontSize)
    {
      this->fontSize = fontSize;
    }
    
    float getFontSize() const
    {
      return fontSize;
    }

  //private: FIXME should be really private
    unsigned char extractAsciiCode(const std::string &paramName) const
    {
      size_t start = paramName.find(' ');
      size_t end   = paramName.find(' ', start + 1);
      
      if (start == std::string::npos || end == std::string::npos) {
        throw GladeException("BitmapFont: Could not read ASCII code");
      }
      
      std::string asciiCodeStr = paramName.substr(start + 1, end - start);
        
      return (unsigned char) ::atoi(asciiCodeStr.c_str());
    }
    
    void setFirstGlyphAsciiCode(char firstGlyphAsciiCode)
    {
      this->firstGlyphAsciiCode = firstGlyphAsciiCode;
    }
    
    void setGlyphHeight(int glyphHeight)
    {
      this->glyphHeight = glyphHeight;
    }
    
    void setGlyphWidth(unsigned char asciiCode, int width)
    {
      if (asciiCode < 0 || asciiCode >= glyphWidths.size()) {
        return;
      }
      
      if (width > cellWidth) {
        width = cellWidth;
      }
      
      glyphWidths[asciiCode] = width;
    }
};
