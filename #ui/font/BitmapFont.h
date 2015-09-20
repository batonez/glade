#pragma once

#include <string>
#include <algorithm>
#include "../../render/Texture.h"
#include "../../util/CSVReader.h"
#include "../../GladeObject.h"
#include "../../render/meshes/Rectangle.h"
#include "../../log/log.h"

class BitmapFont
{
	private:
    Texture* atlas;
	  int cellWidth, cellHeight;
	  int firstGlyphAsciiCode;
	  int glyphHeight;
	  std::vector<int> glyphWidths;
	  int numberOfGlyphsInARow;

   	// Desired result font height in screen coordinates
	  float fontSize;
    
  public:	
    BitmapFont(const char* atlasFilename, const char* csvFilename) :
      fontSize(1)
    {
      if (atlasFilename == NULL || csvFilename == NULL) {
        throw GladeException("Provided atlas data or CSV data is NULL");
      }
      
      std::ifstream csvInput(csvFilename);
      std::vector<std::vector<std::string> > parsedCsv;
      CSVReader::read(csvInput, parsedCsv);
      
      int declaredAtlasWidth  = ::atoi(parsedCsv[0][1].c_str());
      int declaredAtlasHeight = ::atoi(parsedCsv[1][1].c_str());
      cellWidth               = ::atoi(parsedCsv[2][1].c_str());
      cellHeight              = ::atoi(parsedCsv[3][1].c_str());
      
      numberOfGlyphsInARow    = declaredAtlasWidth / cellWidth;
      
      atlas = TextureLoader::loadTexture(atlasFilename, cellWidth, cellHeight);
            
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
      
      log("First glyph ascii code: %d", firstGlyphAsciiCode);
      
      this->setGlyphHeight(::atoi(parsedCsv[6][1].c_str()));
      
      for (int i = 8; i < parsedCsv.size(); ++i) {
        std::string paramName = parsedCsv[i][0];
        std::transform(paramName.begin(), paramName.end(), paramName.begin(), ::tolower);
        
        if (paramName.compare(0, 4, "char") == 0) {
          if (paramName.find("base width") != std::string::npos) {
            unsigned char extractedIndex = extractIndex(paramName);
            log("Extracting \"%s\" => %d, width: %d", paramName.c_str(), extractedIndex, ::atoi(parsedCsv[i][1].c_str()));
            
            this->setGlyphWidth(extractedIndex, ::atoi(parsedCsv[i][1].c_str()));
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
      int glyphIndex;
      Drawable* rectangle = NULL;
      Vector2i glyphPosition;
      float nextOffsetX = 0;
      float halfStringWidth = getStringWidth(string);

      for (size_t i = 0; i < string.length(); ++i) {
        glyphIndex = getGlyphIndexForAsciiCode(string[i]);
        glyphPosition = getGlyphPositionForIndex(glyphIndex);
        
        log("GLYPH: %c, GLYPH ASCII: %d, GLYPH INDEX: %d, GLYPH WIDTH: %d", string[i], string[i], glyphIndex, getGlyphWidth(glyphIndex));
        
        rectangle = new Drawable(Rectangle::INSTANCE);
        rectangle->getTransform()->getScaleP()->x = ((float)getGlyphWidth(glyphIndex) / (float)glyphHeight) * fontSize;
        rectangle->getTransform()->getScaleP()->y = fontSize;
        
        nextOffsetX += rectangle->getTransform()->getScale().x;
        rectangle->getTransform()->getPositionP()->x = nextOffsetX - halfStringWidth;
        nextOffsetX += rectangle->getTransform()->getScale().x;

        rectangle->setTexture(atlas);
       // rectangle->setConstantColor((float)(rand() % 100)/100.0f, (float)(rand() % 100)/100.0f, (float)(rand() % 100)/100.0f, 0.5f);
        rectangle->getTextureTransform()->setCurrentFrameNumber(glyphPosition.x);
        rectangle->getTextureTransform()->setCurrentAnimationNumber(glyphPosition.y);
        rectangle->getTextureTransform()->setCurrentFrameWidth(getGlyphWidth(glyphIndex));
        rectangle->getTextureTransform()->setCurrentFrameHeight(glyphHeight);
        
        text->insert(rectangle);
      }

      return text;
    }
    
    float getStringWidth(const std::string &string)
    {
      float stringWidth = 0;
      
      for (size_t i = 0; i < string.length(); ++i) {
     //   char ch = string[i];
     //   int index = getGlyphIndexForAsciiCode(ch);
     //   float glyphWidth = (float) getGlyphWidth(index);
     //   stringWidth = glyphWidth / (float) glyphHeight * fontSize;
      
        stringWidth += ((float) getGlyphWidth(getGlyphIndexForAsciiCode(string[i]))/(float)glyphHeight) * fontSize;
      }

      return stringWidth;
    }
      
    Texture* getAtlas()
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
    
    int getGlyphWidth(int glyphIndex)
    {
      return glyphWidths[glyphIndex];
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
    unsigned char extractIndex(const std::string &paramName)
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
    
    void setGlyphWidth(int glyphIndex, int width)
    {
      if (glyphIndex < 0 || glyphIndex >= glyphWidths.size()) {
        return;
      }
      
      if (width > cellWidth) {
        width = cellWidth;
      }
      
      glyphWidths[glyphIndex] = width;
    }
};
