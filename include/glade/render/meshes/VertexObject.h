#pragma once

#include <assert.h>
#include "glade/debug/log.h"

class VertexObject {
  public:
    short int vertexVboId, indexVboId;
    
    virtual float* getVertices(void) = 0;    
    virtual unsigned short* getIndices(void) = 0;
    virtual unsigned int getIndexBufferSize(void) = 0;
    virtual unsigned int getVertexBufferSize(void) = 0;
    
    virtual void erase(void) {
      log("Not erasing vertex object for debug purposes");
    }
  
    bool hasVideoBufferHandle()
    {
      if (indexVboId < 0) {
        assert(vertexVboId < 0);
        return false;
      }
      
      return true;
    }
  
  protected:
    VertexObject() :
      vertexVboId(-1),
      indexVboId(-1)
    {
    }
};
