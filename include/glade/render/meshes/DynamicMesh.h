#pragma once

#include <vector>
#include "Mesh.h"

class DynamicMeshGenerator;

namespace Glade {
  class ResourceManager;
}

// FIXME Melt into Mesh class
// FIXME Builtin meshes should inherit this class
class DynamicMesh: public Glade::Mesh
{
  public:
    typedef std::vector<float> Vertices;
    typedef Vertices::iterator VerticesI;

    typedef std::vector<unsigned short> Indices;
    typedef Indices::iterator           IndicesI;

    Vertices vertices;
    Indices indices;

  public:
    DynamicMesh(): Mesh() {}
    
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
};