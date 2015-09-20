#include <glade/util/ResourceManager.h>
#include <glade/util/Path.h>
#include <glade/render/ShaderProgram.h>
#include <glade/render/Texture.h>
#include <glade/audio/Sound.h>
#include <glade/exception/GladeException.h>
#include <glade/opengl/drivers.h>

#ifdef _WIN32 // FIXME these should be crossplatform
#include <glade/util/RIFFReader.h>
#include <lodepng.h>
#endif

Glade::ResourceManager::ResourceManager(FileManager *file_manager):
  fileManager(file_manager)
{
}

// TODO frame width and frame height must be stored within the texture on the disk.
// For now we enforce frame dimensions when the texture is initially loaded from disk
std::shared_ptr<Texture> Glade::ResourceManager::getTexture(const Path &filename, int frameWidth, int frameHeight)
{
  TexturesI i = textures.find(filename);
  
  if (i != textures.end()) {
    return i->second;
  }
  
  std::shared_ptr<Texture> texture = loadTexture(filename, frameWidth, frameHeight);
  textures[filename] = texture;
  return texture;
}

std::shared_ptr<ShaderProgram> Glade::ResourceManager::getShaderProgram(const Path &vertex_shader_filename, const Path &fragment_shader_filename)
{
  Path fullPathToVertexShader = getShadersSubfolder() + vertex_shader_filename;
  Path fullPathToFragmentShader = getShadersSubfolder() + fragment_shader_filename;
  Path key = fullPathToVertexShader + ";" + fullPathToFragmentShader;
  ShaderProgramsI i = shaderPrograms.find(key);
  
  if (i != shaderPrograms.end()) {
    return i->second;
  }
  
  std::shared_ptr<ShaderProgram> shaderProgram = loadShaderProgram(fullPathToVertexShader, fullPathToFragmentShader);
  shaderPrograms[key] = shaderProgram;
  return shaderProgram;
}

Path Glade::ResourceManager::getShadersSubfolder() const
{
  switch (GLADE_VIDEO_DRIVER) {
    case VIDEO_DRIVER_OPENGL3:
      return Path("shaders/gl");
    case VIDEO_DRIVER_OPENGLES2:
      return Path("shaders/gles2");
  }
}

std::shared_ptr<Sound> Glade::ResourceManager::getSound(const Path &filename)
{
  SoundsI i = sounds.find(filename);
  
  if (i != sounds.end()) {
    return i->second;
  }
  
  std::shared_ptr<Sound> sound = loadSound(filename);
  sounds[filename] = sound;
  return sound;
}

std::shared_ptr<ShaderProgram> Glade::ResourceManager::loadShaderProgram(const Path &vertex_shader_filename, const Path &fragment_shader_filename)
{
  std::shared_ptr<ShaderProgram> shaderProgram(new ShaderProgram());

  fileManager->getFileContents(vertex_shader_filename, shaderProgram.get()->vertexShaderSource);
  fileManager->getFileContents(fragment_shader_filename, shaderProgram.get()->fragmentShaderSource);

  return shaderProgram;
}

std::shared_ptr<Texture> Glade::ResourceManager::loadTexture(const Path &filename, int frameWidth, int frameHeight)
{
  if (frameWidth < 0 || frameHeight < 0) {
    throw GladeException("Provided frameWidth and frameHeight must not be less than zero");
  }
  
  std::vector<unsigned char> pngData;
  fileManager->getFileContents(filename, pngData, true);
  
  std::vector<unsigned char> pixels;
  unsigned width, height;

// FIXME (not crossplatform)
#ifdef _WIN32
  unsigned error = lodepng::decode(pixels, width, height, pngData);

  if (error) {
    std::string exceptionText;
    exceptionText += "LodePNG error: ";
    exceptionText += lodepng_error_text(error);
    throw GladeException(exceptionText.c_str());
  }
#endif  
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

std::shared_ptr<Sound> Glade::ResourceManager::loadSound(const Path &filename)
{
  // todo implement
  return std::shared_ptr<Sound>(new Sound());
}
