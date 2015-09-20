#pragma once

#include "VertexObject.h"

class Triangle : public VertexObject
{
  protected:
    Triangle() : VertexObject()
    {
    }
    
  public:
    static Triangle INSTANCE; 
    
    virtual float* getVertices()
    {
      static float vertices[] = {
        0.0f, 0.5f, 0.0f,  // position
        0.0f, 0.0f, 0.0f, // normal
        0.0f, 0.0f, // texcoord

        -0.5f, -0.5f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f,

        0.5f, -0.5f, 0.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f
      };
      
      return vertices;
    }

    virtual unsigned short* getIndices()
    {
      static unsigned short indices[] = { 0, 1, 2 };
      return indices;
    }

    virtual unsigned int getVertexBufferSize()
    {
      return 36;
    }
    
    virtual unsigned int getIndexBufferSize()
    {
      return 3;
    }
    
    virtual void erase()
    {
      
    }
};
