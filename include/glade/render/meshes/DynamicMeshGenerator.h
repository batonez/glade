#pragma once

#include <cmath>
#include "DynamicMesh.h"

class DynamicMeshGenerator
{
  private:
    float        tileSize;
    unsigned int meshSizeTiles;

  public:
    DynamicMeshGenerator():
      tileSize(1.0f),
      meshSizeTiles(10)
    {}
  
    void generate(DynamicMesh &mesh)
    {
      mesh.vertices.clear();
      mesh.indices.clear();
      float halfTileSize = tileSize / 2.0f;
        /*
      static float vertices[] = {
        -halfTileSize, halfTileSize, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f,
        
        halfTileSize, halfTileSize, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f,

        -halfTileSize, -halfTileSize, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f,
        
        halfTileSize, -halfTileSize, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f,
      };

      
    
      static unsigned short indices[] = {
        2, 1, 0,
        2, 0, 3,
      };
      
      for (int i = 0; i < 32; ++i)
        mesh.vertices.push_back(vertices[i]);
      */
      
      int meshSizeVertices = meshSizeTiles + 1;
      int lastVertex = meshSizeVertices * meshSizeVertices - meshSizeVertices;
      float z;
      
      for (int i = 0; i < meshSizeVertices * meshSizeVertices; ++i)
      {
        z = 0.0f + (float) (::rand()) / ((float) (RAND_MAX / (1.0f - 0.0f)));

        // position
        mesh.vertices.push_back(  i % meshSizeVertices);
        mesh.vertices.push_back(- i / meshSizeVertices);
        mesh.vertices.push_back(z);

        // normal (TODO)
        mesh.vertices.push_back(0.0f);
        mesh.vertices.push_back(0.0f);
        mesh.vertices.push_back(1.0f);

        // tex coord (TODO)
        mesh.vertices.push_back(0.0f);
        mesh.vertices.push_back(0.0f);
      }
      
      for (int i = 0; i < lastVertex; ++i)
      {
        if ((i + 1) % meshSizeVertices == 0)
          continue;

        mesh.indices.push_back(i);
        mesh.indices.push_back(i + meshSizeVertices);
        mesh.indices.push_back(i + 1);
        mesh.indices.push_back(i + 1);
        mesh.indices.push_back(i + meshSizeVertices);
        mesh.indices.push_back(i + meshSizeVertices + 1);
      }
    }
};
