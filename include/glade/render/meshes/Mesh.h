#pragma once

#include <assert.h>
#include <vector>
#include "glade/debug/log.h"

namespace Glade {
  class Mesh {
    protected:
      // True indicates the renderer that the mesh may change over time
      bool dynamic;

    public:
      typedef std::vector<float> Vertices;
      typedef Vertices::iterator VerticesI;

      typedef std::vector<unsigned int> Indices;
      typedef Indices::iterator         IndicesI;

      Vertices vertices;
      Indices indices;

      short int vertexVboId, indexVboId;

      // Set these to tell the renderer that it should reupload
      // a part of the mesh into VBO.
      // Note that these values are measured in vertices (not floats or bytes)
      typedef std::pair<int,int>   Region;
      typedef std::vector<Region>  Regions;
      typedef Regions::iterator    RegionsI;

      Regions updateRegions;

      Mesh(bool dynamic_param = false) :
        vertexVboId(-1),
        indexVboId(-1),
        dynamic(dynamic_param)
      {}

      const Mesh& operator=(const Mesh& rhs)
      {
        vertices.insert(vertices.begin(), rhs.vertices.begin(), rhs.vertices.end());
        indices.insert(indices.begin(), rhs.indices.begin(), rhs.indices.end());
      }

      virtual void calculateNormals();
      virtual void invertNormals();

      virtual float* getVertices()
      {
        return vertices.data();
      }

      virtual unsigned int* getIndices()
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
        vertices.clear();
        indices.clear();
      }

      bool isDynamic() {
        return dynamic;
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