#pragma once

#include "meshes/VertexObject.h"
#include "Texture.h"
#include "TextureTransform.h"
#include "Material.h"
#include "ShaderProgram.h"

class Drawable {
public:
	bool preservePosition, preserveRotation, preserveScale;
	
protected:
	Transform transform;
	VertexObject* vertexObject;
  Texture* texture;
  ShaderProgram *shaderProgram;
  TextureTransform textureTransform;
  
public:
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