#pragma once

#include <assert.h>
#include <vector>
#include "glade/debug/log.h"

namespace Glade {
  class Mesh {
    public:
      Mesh() :
        vertexVboId(-1),
        indexVboId(-1),
        vaoId(-1)
      {
      }

      typedef std::vector<float> Vertices;
      typedef Vertices::iterator VerticesI;

      typedef std::vector<unsigned short> Indices;
      typedef Indices::iterator           IndicesI;

      short int vertexVboId, indexVboId, vaoId;
      Vertices vertices;
      Indices indices;
      
      virtual float* getVertices()
      {
        return vertices.data();
      }

      virtual unsigned short* getIndices()
      {
        return indices.data();
      }

      virtual unsigned int getVertexBufferSize()
      {
        return vertices.size();
      }

      virtual unsigned int getIndexBufferSize()
      {
        return indices.size();
      }
      
      virtual void erase(void) {
        log("FIXME: Not erasing mesh for no reason");
      }
    
      bool hasVideoBufferHandle()
      {
        if (indexVboId < 0) {
          assert(vertexVboId < 0);
          return false;
        }
        
        return true;
      }
    
  };
}