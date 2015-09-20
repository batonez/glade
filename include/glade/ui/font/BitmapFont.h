#pragma once

#include <string>
#include <algorithm>
#include <fstream> // crap
#include <memory>

#include "glade/render/Texture.h"
#include "glade/util/CSVReader.h"
#include "glade/util/ResourceManager.h"
#include "glade/GladeObject.h"
#include "glade/render/meshes/Rectangle.h"

extern ResourceManager resource_manager;

class BitmapFont
{
	private:
    std::shared_ptr<Texture> atlas;
    std::shared_ptr<ShaderProgram> shaderProgram;
	  int cellWidth, cellHeight;
	  int firstGlyphAsciiCode;
	  int glyphHeight;
	  std::vector<int> glyphWidths;
	  int numberOfGlyphsInARow;

   	// Desired result font height in screen coordinates
	  float fontSize;
    
  public:
    // TODO move loading into resource manager
    BitmapFont(const char* atlas_filename, const char* csv_filename, std::shared_ptr<ShaderProgram> &shader_program):
      fontSize(1)
    {
      if (atlas_filename == NULL || csv_filename == NULL || shader_program.get() == nullptr) {
        throw GladeException("Provided atlas data or CSV data is NULL");
      }
      
      shaderProgram = shader_program;
      
      std::ifstream csvInput(csv_filename);
      std::vector<std::vector<std::string> > parsedCsv;
      CSVReader::read(csvInput, parsedCsv);
      
      int declaredAtlasWidth  = ::atoi(parsedCsv[0][1].c_str());
      int declaredAtlasHeight = ::atoi(parsedCsv[1][1].c_str());
      cellWidth               = ::atoi(parsedCsv[2][1].c_str());
      cellHeight              = ::atoi(parsedCsv[3][1].c_str());
      
      numberOfGlyphsInARow    = declaredAtlasWidth / cellWidth;
      
      atlas = resource_manager.getTexture(atlas_filename, cellWidth, cellHeight);
            
      if (declaredAtlasWidth != atlas->textureWidth) {
        throw GladeException("CSV data doesn't match atlas data");
      }
      
      if (declaredAtlasHeight != atlas->textureHeight) {
        throw GladeException("CSV data doesn't match atlas data");
      }
      
      if (cellWidth == 0 || cellHeight == 0 || atlas == NULL || cellWidth > atlas->textureWidth || cellHeight > atlas->textureHeight) {
        throw GladeException("Invalid values for BitmapFont constructor");
      }
      
      glyphWidths.resize(256, cellWidth);
      
      this->setFirstGlyphAsciiCode((unsigned char) ::atoi((parsedCsv[4][1].c_str())));
      
      this->setGlyphHeight(::atoi(parsedCsv[6][1].c_str()));
      
      for (int i = 8; i < parsedCsv.size(); ++i) {
        std::string paramName = parsedCsv[i][0];
        std::transform(paramName.begin(), paramName.end(), paramName.begin(), ::tolower);
        
        if (paramName.compare(0, 4, "char") == 0) {
          if (paramName.find("base width") != std::string::npos) {
            unsigned char extractedAsciiCode = extractAsciiCode(paramName);
            
            this->setGlyphWidth(extractedAsciiCode, ::atoi(parsedCsv[i][1].c_str()));
          }
        }
      }
    }
    
    unsigned getGlyphIndexForAsciiCode(unsigned char asciiCode)
    {
      unsigned glyphIndex = asciiCode - firstGlyphAsciiCode;
      
      if (glyphIndex < 0 || glyphIndex >= glyphWidths.size()) {
        glyphIndex = glyphWidths.size() - 1;
      }
      
      return glyphIndex;
    }
    
    Vector2i getGlyphPositionForIndex(int glyphIndex)
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
    GladeObject::Drawables* createDrawablesForString(std::string &string)
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
        
        text->insert(rectangle);
      }

      return text;
    }
    
    float getStringWidth(const std::string &string)
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
    
    int getCellWidth()
    {
      return cellWidth;
    }
    
    int getCellHeight()
    {
      return cellHeight;
    }
    
    int getGlyphWidth(unsigned char asciiCode)
    {
      return glyphWidths[asciiCode];
    }
    
    int getGlyphHeight()
    {
      return glyphHeight;
    }
    
    int getNumberOfChars()
    {
      return glyphWidths.size();
    }
    
    void setFontSize(float fontSize)
    {
      this->fontSize = fontSize;
    }
    
    float getFontSize()
    {
      return fontSize;
    }

  private:
    unsigned char extractAsciiCode(const std::string &paramName)
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
