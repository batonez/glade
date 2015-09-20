#pragma once

#include "meshes/VertexObject.h"
#include "Texture.h"
#include "TextureTransform.h"
#include "Material.h"

class Drawable {
public:
	bool preservePosition, preserveRotation, preserveScale;
	bool replaceOriginalColor;
    unsigned int vertexVboId, indexVboId;
    bool initialized;
	
protected:
	Transform transform;
	VertexObject* vertexObject;
  Texture* texture;
  TextureTransform textureTransform;
  Material material;
  Vector4f constantColor;
  bool lit;
    
public:
  explicit Drawable(VertexObject &vertObject):
  vertexObject(&vertObject),
  preservePosition(false),
  preserveRotation(false),
  preserveScale(false),
  replaceOriginalColor(false),
  vertexVboId(0), // check if 0 can be a valid VBO ID
  indexVboId (0),  // check if 0 can be a valid VBO ID
  initialized(false),
  lit(false),
  texture(NULL)
  {}
  
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
  
  bool isInitialized(void) {
    return initialized;
  }
  
  void setConstantColor(float r, float g, float b) {
    constantColor.set(r, g, b, 1.0f);
  }
  
  void setConstantColor(float r, float g, float b, float a) {
    constantColor.set(r, g, b, a);
  }
  
  Vector4f* getConstantColor(void) {
    return &constantColor;
  }
    
	bool isLit(void) {
		return lit;
	}
	
	void setLit(bool lit) {
		this->lit = lit;
	}
	
	Material* getMaterial(void) {
		return &material;
	}
	
	void setMaterial(const Material &material) {
		this->material = material;
	}
	
	VertexObject* getVertexObject(void) {
		return vertexObject;
	}
};