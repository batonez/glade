#pragma once

#include <string>
#include <map>
#include <memory>

#include "glade/math/Transform.h"
#include "meshes/Mesh.h"
#include "Texture.h"
#include "TextureTransform.h"
#include "ShaderProgram.h"

class Drawable {
public:
  typedef std::map<std::string, float>    ShaderFloatUniforms;
  typedef std::map<std::string, bool>     ShaderBoolUniforms;
  typedef std::map<std::string, int>      ShaderIntUniforms;
  typedef std::map<std::string, Vector3f> ShaderVec3Uniforms;
  typedef std::map<std::string, Vector4f> ShaderVec4Uniforms;
  
  typedef ShaderFloatUniforms::const_iterator ShaderFloatUniformsCI;
  typedef ShaderBoolUniforms::const_iterator ShaderBoolUniformsCI;
  typedef ShaderIntUniforms::const_iterator ShaderIntUniformsCI;
  typedef ShaderVec3Uniforms::const_iterator ShaderVec3UniformsCI;
  typedef ShaderVec4Uniforms::const_iterator ShaderVec4UniformsCI;
  
  bool preservePosition, preserveRotation, preserveScale;
  
protected:
  Transform transform;
  std::shared_ptr<Glade::Mesh> mesh;
  std::shared_ptr<Texture> texture;
  TextureTransform textureTransform;
  std::shared_ptr<ShaderProgram> shaderProgram;
  
private:
  ShaderFloatUniforms shaderFloatUniforms;
  ShaderBoolUniforms  shaderBoolUniforms;
  ShaderIntUniforms   shaderIntUniforms;
  ShaderVec3Uniforms  shaderVec3Uniforms;
  ShaderVec4Uniforms  shaderVec4Uniforms;
  
public:
  Drawable():
    preservePosition(false),
    preserveRotation(false),
    preserveScale(false)
  {}

  Drawable(std::shared_ptr<Glade::Mesh> mesh, std::shared_ptr<ShaderProgram> shaderProgram):
    mesh(mesh),
    shaderProgram(shaderProgram),
    preservePosition(false),
    preserveRotation(false),
    preserveScale(false)
  {
  }
  
  ShaderFloatUniformsCI floatUniformsBegin()
  {
    return shaderFloatUniforms.cbegin();
  }
  
  ShaderFloatUniformsCI floatUniformsEnd()
  {
    return shaderFloatUniforms.cend();
  }
  
  ShaderBoolUniformsCI boolUniformsBegin()
  {
    return shaderBoolUniforms.cbegin();
  }
  
  ShaderBoolUniformsCI boolUniformsEnd()
  {
    return shaderBoolUniforms.cend();
  }
  
  ShaderIntUniformsCI intUniformsBegin()
  {
    return shaderIntUniforms.cbegin();
  }
  
  ShaderIntUniformsCI intUniformsEnd()
  {
    return shaderIntUniforms.cend();
  }

  ShaderVec3UniformsCI vec3UniformsBegin()
  {
    return shaderVec3Uniforms.cbegin();
  }
  
  ShaderVec3UniformsCI vec3UniformsEnd()
  {
    return shaderVec3Uniforms.cend();
  }
  
  ShaderVec4UniformsCI vec4UniformsBegin()
  {
    return shaderVec4Uniforms.cbegin();
  }
  
  ShaderVec4UniformsCI vec4UniformsEnd()
  {
    return shaderVec4Uniforms.cend();
  }
  
  void setUniform(const std::string &name, float val)
  {
    shaderFloatUniforms[name] = val;
  }
  
  void setUniform(const std::string &name, bool val)
  {
    shaderBoolUniforms[name] = val;
  }
  
  void setUniform(const std::string &name, int val)
  {
    shaderIntUniforms[name] = val;
  }
  
  void setUniform(const std::string &name, const Vector3f &val)
  {
    shaderVec3Uniforms[name] = val;
  }
  
  void setUniform(const std::string &name, const Vector4f val)
  {
    shaderVec4Uniforms[name] = val;
  }
  
  std::shared_ptr<ShaderProgram> getShaderProgram()
  {
    return shaderProgram;
  }
  
  void setTexture(std::shared_ptr<Texture> texture) {
    this->texture = texture;
  }
  
  std::shared_ptr<Texture> getTexture(void) {
    return texture;
  }
  
  void setTextureTransform(const TextureTransform &texTransform) {
    textureTransform = texTransform;
  }
  
  TextureTransform* getTextureTransform(void) {
    return &textureTransform;
  }
  
  void setTransform(const Transform &transform) {
    this->transform = transform;
  }
  
  Transform* getTransform(void) {
    return &transform;
  }
  
  std::shared_ptr<Glade::Mesh> getMesh(void) {
    return mesh;
  }
};
