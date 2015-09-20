#pragma once

#include <memory>
#include <lodepng.h>
#include <unordered_map>

#include "Path.h"
#include "FileManager.h"
#include "glade/render/ShaderProgram.h"
#include "glade/render/Texture.h"
#include "glade/audio/Sound.h"
#include "glade/util/RIFFReader.h"
#include "glade/exception/GladeException.h"

namespace Glade
{
  class ResourceManager
  {
    protected:
      FileManager fileManager;
      
    private:
      typedef std::unordered_map<Path, std::shared_ptr<Texture>, Path::Hasher>        Textures;
      typedef Textures::iterator                                                      TexturesI;
      
      typedef std::unordered_map<Path, std::shared_ptr<ShaderProgram>, Path::Hasher>  ShaderPrograms;
      typedef ShaderPrograms::iterator                                                ShaderProgramsI;
      
      typedef std::unordered_map<Path, std::shared_ptr<Sound>, Path::Hasher>          Sounds;
      typedef Sounds::iterator                                                        SoundsI;
      
      Textures        textures;
      ShaderPrograms  shaderPrograms;
      Sounds          sounds;
 
    public:
      ResourceManager(const Path &base_path):
        textures(0, Path::hasher),
        shaderPrograms (0, Path::hasher)
      {
        fileManager.setBasePath(base_path);
      }
      
      // TODO frame width and frame height must be stored within the texture on the disk.
      // For now we enforce frame dimensions when the texture is initially loaded from disk
      std::shared_ptr<Texture> getTexture(const Path &filename, int frameWidth = 0, int frameHeight = 0)
      {
        TexturesI i = textures.find(filename);
        
        if (i != textures.end()) {
          return i->second;
        }
        
        std::shared_ptr<Texture> texture = loadTexture(filename, frameWidth, frameHeight);
        textures[filename] = texture;
        return texture;
      }
      
      std::shared_ptr<ShaderProgram> getShaderProgram(const Path &vertex_shader_filename, const Path &fragment_shader_filename)
      {
        Path key = vertex_shader_filename + ";" + fragment_shader_filename;
        ShaderProgramsI i = shaderPrograms.find(key);
        
        if (i != shaderPrograms.end()) {
          return i->second;
        }
        
        std::shared_ptr<ShaderProgram> shaderProgram = loadShaderProgram(vertex_shader_filename, fragment_shader_filename);
        shaderPrograms[key] = shaderProgram;
        return shaderProgram;
      }
    
      std::shared_ptr<Sound> getSound(const Path &filename)
      {
        SoundsI i = sounds.find(filename);
        
        if (i != sounds.end()) {
          return i->second;
        }
        
        std::shared_ptr<Sound> sound = loadSound(filename);
        sounds[filename] = sound;
        return sound;
      }
    
    private:
      std::shared_ptr<ShaderProgram> loadShaderProgram(const Path &vertex_shader_filename, const Path &fragment_shader_filename)
      {
        std::shared_ptr<ShaderProgram> shaderProgram(new ShaderProgram());
        
        fileManager.getFileContents<char>(vertex_shader_filename, shaderProgram.get()->vertexShaderSource);
        fileManager.getFileContents<char>(fragment_shader_filename, shaderProgram.get()->fragmentShaderSource);
        
        return shaderProgram;
      }
      
      std::shared_ptr<Texture> loadTexture(const Path &filename, int frameWidth = 0, int frameHeight = 0)
      {
        if (frameWidth < 0 || frameHeight < 0) {
          throw GladeException("Provided frameWidth and frameHeight must not be less than zero");
        }
        
        std::vector<unsigned char> pngData;
        fileManager.getFileContents<unsigned char>(filename, pngData, true);
        
        std::vector<unsigned char> pixels;
        unsigned width, height;
     
        unsigned error = lodepng::decode(pixels, width, height, pngData);

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
        
        return std::shared_ptr<Texture>(new Texture(
          width,
          height,
          numberOfAnimations,
          numberOfFrames,
          pixels
        ));
      }
      
      std::shared_ptr<Sound> loadSound(const Path &filename)
      {
        // todo implement
        return std::shared_ptr<Sound>(new Sound());
      }
  };
}