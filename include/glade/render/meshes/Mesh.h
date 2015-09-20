#pragma once

#include <assert.h>
#include "glade/debug/log.h"

namespace Glade {
  class Mesh {
    public:
      short int vertexVboId, indexVboId;
      
      virtual float* getVertices(void) = 0;    
      virtual unsigned short* getIndices(void) = 0;
      virtual unsigned int getIndexBufferSize(void) = 0;
      virtual unsigned int getVertexBufferSize(void) = 0;
      
      virtual void erase(void) {
        log("FIXME: Not erasing mesh for debug purposes");
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
      Mesh() :
        vertexVboId(-1),
        indexVboId(-1)
      {
      }
  };
}