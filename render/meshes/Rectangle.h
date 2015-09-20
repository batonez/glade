#pragma once

#include "VertexObject.h"

class Rectangle : public VertexObject
{
protected:
  Rectangle() {}
  
public:
  static Rectangle INSTANCE; 
  
  virtual float* getVertices()
	{
    static float vertices[] = { 
      -1, -1, 0,
      0, 0, 0,
      0, 0,
      
      1, -1, 0,
      0, 0, 0,
      1, 0, 
      
      1, 1, 0,
      0, 0, 0,
      1, 1,
      
      -1, 1, 0,
      0, 0, 0,
      0, 1,   
    };
  		
		return vertices;
	}

	virtual unsigned short* getIndices()
	{
		static unsigned short indices[] = { 2, 1, 0, 3, 2, 0 };
		return indices;
	}

  virtual unsigned int getVertexBufferSize()
  {
    return 48;
  }
  
  virtual unsigned int getIndexBufferSize()
  {
    return 6;
  }
  
	virtual void erase()
	{
		
	}
};
