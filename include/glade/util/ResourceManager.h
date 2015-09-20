#pragma once

#include <memory>
#include <map>

#include "FileManager.h"

class Path;
class ShaderProgram;
class Texture;
class Sound;

namespace Glade
{
  class ResourceManager
  {
    protected:
      FileManager *fileManager;
      
    private:
      typedef std::map<Path, std::shared_ptr<Texture> >        Textures;
      typedef Textures::iterator                               TexturesI;
      
      typedef std::map<Path, std::shared_ptr<ShaderProgram> >  ShaderPrograms;
      typedef ShaderPrograms::iterator                         ShaderProgramsI;
      
      typedef std::map<Path, std::shared_ptr<Sound> >          Sounds;
      typedef Sounds::iterator                                 SoundsI;
      
      Textures        textures;
      ShaderPrograms  shaderPrograms;
      Sounds          sounds;
 
    public:
      ResourceManager(FileManager *file_manager);
      std::shared_ptr<Texture> getTexture(const Path &filename, int frameWidth = 0, int frameHeight = 0);
      std::shared_ptr<ShaderProgram> getShaderProgram(const Path &vertex_shader_filename, const Path &fragment_shader_filename);
      std::shared_ptr<Sound> getSound(const Path &filename);
    
    private:
      std::shared_ptr<ShaderProgram> loadShaderProgram(const Path &vertex_shader_filename, const Path &fragment_shader_filename);
      std::shared_ptr<Texture> loadTexture(const Path &filename, int frameWidth = 0, int frameHeight = 0);
      std::shared_ptr<Sound> loadSound(const Path &filename);
  };
}
