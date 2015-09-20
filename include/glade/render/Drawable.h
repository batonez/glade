#pragma once

#include <string>
#include <unordered_map>

#include "meshes/VertexObject.h"
#include "Texture.h"
#include "TextureTransform.h"
#include "Material.h"
#include "ShaderProgram.h"
#include "../math/Transform.h"

class Drawable {
public:
	bool preservePosition, preserveRotation, preserveScale;
	
protected:
	Transform transform;
	VertexObject* vertexObject;
  Texture* texture;
  TextureTransform textureTransform;
  ShaderProgram *shaderProgram;
  
public:
  typedef std::unordered_map<std::string, float>  ShaderFloatUniforms;
  typedef std::unordered_map<std::string, bool>   ShaderBoolUniforms;
  typedef std::unordered_map<std::string, int>    ShaderIntUniforms;
  typedef std::unordered_map<std::string, Vector3f> ShaderVec3Uniforms;
  typedef std::unordered_map<std::string, Vector4f> ShaderVec4Uniforms;
  ShaderFloatUniforms shaderFloatUniforms;
  ShaderBoolUniforms  shaderBoolUniforms;
  ShaderIntUniforms   shaderIntUniforms;
  ShaderVec3Uniforms  shaderVec3Uniforms;
  ShaderVec4Uniforms  shaderVec4Uniforms;

  Drawable(VertexObject &vertObject, ShaderProgram &shaderProgram):
    vertexObject(&vertObject),
    shaderProgram(&shaderProgram),
    preservePosition(false),
    preserveRotation(false),
    preserveScale(false),
    texture(NULL)
  {
  }
  
  ShaderProgram* getShaderProgram()
  {
    return shaderProgram;
  }
  
  void setTexture(Texture* texture) {
    this->texture = texture;
  }
  
  Texture* getTexture(void) {
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
	
	VertexObject* getVertexObject(void) {
		return vertexObject;
	}
};