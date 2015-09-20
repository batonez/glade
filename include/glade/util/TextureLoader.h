#pragma once

#include <lodepng.h>
#include "../exception/GladeException.h"

class TextureLoader
{
  public:
    static Texture* loadTexture(const char* filename, int frameWidth, int frameHeight)
    {
      if (filename == NULL) {
        throw GladeException("Provided bitmap data stream is null, texture could not be loaded");
      }
      
      if (frameWidth < 0 || frameHeight < 0) {
        throw GladeException("Provided frameWidth and frameHeight must not be less than zero");
      }
      
      std::vector<unsigned char> pixels;
      unsigned width, height;

      unsigned error = lodepng::decode(pixels, width, height, filename);

      if (error) {
        std::string exceptionText;
        exceptionText += "LodePNG error: ";
        exceptionText += lodepng_error_text(error);
        throw GladeException(exceptionText.c_str());
      }
      
      if (frameWidth == 0) {  // if the user doesn't know frame dimensions
        frameWidth = width;   // we assume the whole texture is one frame
      } 
      
      if (frameHeight == 0) {
        frameHeight = height;
      }
      
      int numberOfFrames = width / frameWidth;
      numberOfFrames = numberOfFrames < 1 ? 1 : numberOfFrames;
      
      int numberOfAnimations = height / frameHeight;
      numberOfAnimations = numberOfAnimations < 1 ? 1 : numberOfAnimations;
      
      Texture* result = new Texture(
        width,
        height,
        numberOfAnimations,
        numberOfFrames,
        pixels
      );
      
      return result;
    }
    
    static Texture* loadTexture(const char* filename)
    {
      return loadTexture(filename, 0, 0);
    }
};
