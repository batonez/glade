#include <glade/util/ResourceManager.h>
#include <glade/util/Path.h>
#include <glade/render/ShaderProgram.h>
#include <glade/render/Texture.h>
#include <glade/ui/font/BitmapFont.h>
#include <glade/audio/Sound.h>
#include <glade/exception/GladeException.h>

#include <lodepng.h>

#ifdef _WIN32 // FIXME these should be crossplatform
#include <glade/util/RIFFReader.h>
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
  Path key = vertex_shader_filename + ";" + fragment_shader_filename;
  ShaderProgramsI i = shaderPrograms.find(key);
  
  if (i != shaderPrograms.end()) {
    return i->second;
  }
  
  std::shared_ptr<ShaderProgram> shaderProgram = loadShaderProgram(vertex_shader_filename, fragment_shader_filename);
  shaderPrograms[key] = shaderProgram;
  return shaderProgram;
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

std::shared_ptr<BitmapFont> Glade::ResourceManager::getFont(const Path &atlas_filename, const Path &csv_filename)
{
  Path key = atlas_filename + ";" + csv_filename;
  BitmapFontsI i = bitmapFonts.find(key);
  
  if (i != bitmapFonts.end()) {
    return i->second;
  }
  
  std::shared_ptr<BitmapFont> bitmapFont = loadFont(atlas_filename, csv_filename);
  bitmapFonts[key] = bitmapFont;
  return bitmapFont;
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

std::shared_ptr<BitmapFont> Glade::ResourceManager::loadFont(const Path &atlas_filename, const Path &csv_filename)
{
  std::vector<char> rawCsv;
  fileManager->getFileContents(csv_filename, rawCsv);
  std::vector<std::vector<std::string> > parsedCsv;
  CSVReader::read(rawCsv, parsedCsv);
  
  int declaredAtlasWidth  = ::atoi(parsedCsv[0][1].c_str());
  int declaredAtlasHeight = ::atoi(parsedCsv[1][1].c_str());
  int cellWidth           = ::atoi(parsedCsv[2][1].c_str());
  int cellHeight          = ::atoi(parsedCsv[3][1].c_str());
  
  std::shared_ptr<Texture> atlas = getTexture(atlas_filename, cellWidth, cellHeight);
  std::shared_ptr<BitmapFont> result(new BitmapFont(atlas, cellWidth, cellHeight));
        
  if (declaredAtlasWidth != atlas->textureWidth) {
    throw GladeException("CSV data doesn't match atlas data");
  }
  
  if (declaredAtlasHeight != atlas->textureHeight) {
    throw GladeException("CSV data doesn't match atlas data");
  }
  
  if (cellWidth == 0 || cellHeight == 0 || atlas == NULL || cellWidth > atlas->textureWidth || cellHeight > atlas->textureHeight) {
    throw GladeException("Invalid values for BitmapFont constructor");
  }
  
  result->numberOfGlyphsInARow    = declaredAtlasWidth / cellWidth;
  result->glyphWidths.resize(256, cellWidth);
  result->setFirstGlyphAsciiCode((unsigned char) ::atoi((parsedCsv[4][1].c_str())));
  result->setGlyphHeight(::atoi(parsedCsv[6][1].c_str()));
  
  for (int i = 8; i < parsedCsv.size(); ++i) {
    std::string paramName = parsedCsv[i][0];
    std::transform(paramName.begin(), paramName.end(), paramName.begin(), ::tolower);
    
    if (paramName.compare(0, 4, "char") == 0) {
      if (paramName.find("base width") != std::string::npos) {
        unsigned char extractedAsciiCode = result->extractAsciiCode(paramName);
        
        result->setGlyphWidth(extractedAsciiCode, ::atoi(parsedCsv[i][1].c_str()));
      }
    }
  }
  
  return result;
}

std::shared_ptr<Sound> Glade::ResourceManager::loadSound(const Path &filename)
{
  // todo implement
  return std::shared_ptr<Sound>(new Sound());
}
