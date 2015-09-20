#include "VertexObject.h"

class Cube : public VertexObject
{
  protected:
    Cube() : VertexObject()
    {
    }
    
  public:
    static Cube INSTANCE; 
    
    virtual float* getVertices()
    {
      static float vertices[] = {
        0.5f, 0.5f, -0.5f,
        0.577349f, 0.577349f, -0.577349f,
        0.000000f, 0.000000f,

        0.5f, -0.5f, -0.5f,
        0.577349f, -0.577349f, -0.577349f,
        0.000000f, 1.000000f,

        -0.5f, -0.5f, -0.5f,
        -0.577349f, -0.577349f, -0.577349f,
        1.000000f, 0.000000f,

        -0.5f, 0.5f, -0.5f,
        -0.577349f, 0.577349f, -0.577349f,
        1.000000f, 1.000000f,

        0.5f, 0.5f, 0.5f,
        0.577349f, 0.577349f, 0.577349f,
        0.000000f, 0.000000f,

        0.5f, -0.5f, 0.5f,
        0.577349f, -0.577349f, 0.577349f,
        0.000000f, 1.000000f,

        -0.5f, -0.5f, 0.5f,
        -0.577349f, -0.577349f, 0.577349f,
        1.000000f, 0.000000f,

        -0.5f, 0.5f, 0.5f,
        -0.577349f, 0.577349f, 0.577349f,
        1.000000f, 1.000000f
      };
      
      return vertices;
    }

    virtual unsigned short* getIndices()
    {
      static unsigned short indices[] = {
        0, 1, 2,
        4, 7, 5,
        0, 4, 1,
        1, 5, 2,
        2, 6, 7,
        4, 0, 3,
        3, 0, 2,
        4, 5, 1,
        5, 6, 2,
        3, 2, 7,
        7, 4, 3,
        7, 6, 5 
      };
      
      return indices;
    }

    virtual unsigned int getVertexBufferSize()
    {
      return 96;
    }
    
    virtual unsigned int getIndexBufferSize()
    {
      return 36;
    }
    
    virtual void erase()
    {
      
    }
};
