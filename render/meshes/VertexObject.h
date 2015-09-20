#pragma once

class VertexObject {
public:
	virtual float* getVertices(void) = 0;    
  virtual unsigned short* getIndices(void) = 0;
  virtual unsigned int getIndexBufferSize(void) = 0;
	virtual unsigned int getVertexBufferSize(void) = 0;
	virtual void erase(void) = 0;
};
