#pragma once

#include <cmath>
#include "Mesh.h"

class MeshGenerator
{
  private:
    static int const    VERTEX_SIZE = 8;

    float        tileSize;
    unsigned int meshSizeTiles;

  public:
    MeshGenerator():
      tileSize(0.5f),
      meshSizeTiles(30)
    {}
  
    void extractVertexCoordsFromArray(int vertex_number, Glade::Mesh::Vertices &vertices, Vector3f &result)
    {
      result.x = vertices[vertex_number * VERTEX_SIZE + 0];
      result.y = vertices[vertex_number * VERTEX_SIZE + 1];
      result.z = vertices[vertex_number * VERTEX_SIZE + 2];
    }
  
    void surfaceNormalFromThreeVertices(Vector3f &a, Vector3f &b, Vector3f &c, Vector3f &result)
    {
      Vector3f first, second;
      first.set(a);
      first.subtract(b);
      second.set(a);
      second.subtract(c);
      first.cross(second, result);
    }
  
    void generate(Glade::Mesh &mesh)
    {
      mesh.vertices.clear();
      mesh.indices.clear();
      
      int meshSizeVertices = meshSizeTiles + 1;
      
      // calculate vertex positions
      float z;
      
      for (int i = 0; i < meshSizeVertices * meshSizeVertices; ++i)
      {
        z = 0.0f + (float) (::rand()) / ((float) (RAND_MAX / (1.0f - 0.0f)));

        // position
        mesh.vertices.push_back(  i % meshSizeVertices);
        mesh.vertices.push_back(- i / meshSizeVertices);
        mesh.vertices.push_back(z);

        // normals will be calculated on the second path
        mesh.vertices.push_back(0.0f);
        mesh.vertices.push_back(0.0f);
        mesh.vertices.push_back(1.0f);

        // tex coord (TODO)
        mesh.vertices.push_back(0.0f);
        mesh.vertices.push_back(0.0f);
      }

      // calculate normals (TODO border & corner cases)

      for (int i = 0; i < meshSizeVertices * meshSizeVertices; ++i)
      {
        bool leftBorder   = false,
             rightBorder  = false,
             bottomBorder = false,
             topBorder    = false;
        // left border
        if ((i + 1) % meshSizeVertices == 0)
          leftBorder = true;

        // right border
        if (i  % meshSizeVertices == 0)
          rightBorder = true;
        
        // bottom border
        if (i / meshSizeVertices == meshSizeVertices - 1)
          bottomBorder = true;
        
        // top border
        if (i / meshSizeVertices == 0)
          topBorder = true;

       // Triangles adjancent to current vertex
       // (components are vertex numbers and should be multiplied by the size of the data of one vertex):
       // A:(i - meshSizeVertices    , i - 1                   , i                       )
       // B:(i - meshSizeVertices    , i                       , i - meshSizeVertices + 1)
       // C:(i - meshSizeVertices + 1, i                       , i + 1                   )
       // D:(i                       , i + meshSizeVertices    , i + 1                   )
       // E:(i                       , i + meshSizeVertices - 1, i + meshSizeVertices    )
       // F:(i - 1                   , i + meshSizeVertices - 1, i                       )

        // calculate normals of each adjancent triangle
        // TODO Optimize, perhaps. Normals are being calculated many times for each triangle

        Vector3f v1, v2, v3, nA, nB, nC, nD, nE, nF;
        nA.setZero();
        nB.setZero();
        nC.setZero();
        nD.setZero();
        nE.setZero();
        nF.setZero();

        if (!leftBorder && !topBorder) {
          extractVertexCoordsFromArray(i - meshSizeVertices    , mesh.vertices, v1);
          extractVertexCoordsFromArray(i - 1                   , mesh.vertices, v2);
          extractVertexCoordsFromArray(i                       , mesh.vertices, v3);
          surfaceNormalFromThreeVertices(v1, v2, v3, nA);
        }

        if (!rightBorder && !topBorder) {
          extractVertexCoordsFromArray(i - meshSizeVertices    , mesh.vertices, v1);
          extractVertexCoordsFromArray(i                       , mesh.vertices, v2);
          extractVertexCoordsFromArray(i - meshSizeVertices + 1, mesh.vertices, v3);
          surfaceNormalFromThreeVertices(v1, v2, v3, nB);
        
          extractVertexCoordsFromArray(i - meshSizeVertices + 1, mesh.vertices, v1);
          extractVertexCoordsFromArray(i                       , mesh.vertices, v2);
          extractVertexCoordsFromArray(i + 1                   , mesh.vertices, v3);
          surfaceNormalFromThreeVertices(v1, v2, v3, nC);
        }
        
        if (!rightBorder && !bottomBorder) {
          extractVertexCoordsFromArray(i                       , mesh.vertices, v1);
          extractVertexCoordsFromArray(i + meshSizeVertices    , mesh.vertices, v2);
          extractVertexCoordsFromArray(i + 1                   , mesh.vertices, v3);
          surfaceNormalFromThreeVertices(v1, v2, v3, nD);
        }
        
        if (!leftBorder && !bottomBorder) {
          extractVertexCoordsFromArray(i                       , mesh.vertices, v1);
          extractVertexCoordsFromArray(i + meshSizeVertices - 1, mesh.vertices, v2);
          extractVertexCoordsFromArray(i + meshSizeVertices    , mesh.vertices, v3);
          surfaceNormalFromThreeVertices(v1, v2, v3, nE);
          
          extractVertexCoordsFromArray(i - 1                   , mesh.vertices, v1);
          extractVertexCoordsFromArray(i + meshSizeVertices - 1, mesh.vertices, v2);
          extractVertexCoordsFromArray(i                       , mesh.vertices, v3);
          surfaceNormalFromThreeVertices(v1, v2, v3, nF);
        }
        
        // sum all triangle normals
        Vector3f currentVertexNormal;
        currentVertexNormal.setZero();
        currentVertexNormal.add(nA);
        currentVertexNormal.add(nB);
        currentVertexNormal.add(nC);
        currentVertexNormal.add(nD);
        currentVertexNormal.add(nE);
        currentVertexNormal.add(nF);
        currentVertexNormal.normalize();
        
        // set the normal to vertex
        mesh.vertices[i * VERTEX_SIZE + 3] = currentVertexNormal.x;
        mesh.vertices[i * VERTEX_SIZE + 4] = currentVertexNormal.y;
        mesh.vertices[i * VERTEX_SIZE + 5] = currentVertexNormal.z;
      }
      
      // fill index array
      int lastVertex = meshSizeVertices * meshSizeVertices - meshSizeVertices;

      for (int i = 0; i < lastVertex; ++i)
      {
        if ((i + 1) % meshSizeVertices == 0)
          continue;

        // upper triangle
        mesh.indices.push_back(i);
        mesh.indices.push_back(i + meshSizeVertices);
        mesh.indices.push_back(i + 1);

        // lower triangle
        mesh.indices.push_back(i + 1);
        mesh.indices.push_back(i + meshSizeVertices);
        mesh.indices.push_back(i + meshSizeVertices + 1);
      }
    }
};
