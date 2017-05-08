#pragma once

#include "Mesh.h"

namespace Glade {
  class Rectangle: public Mesh
  {
    public:
      Rectangle() : Mesh() {
        vertices.insert(vertices.begin(), {
          -1.0, -1.0, 0.0,
          0.0, 0.0, 0.0, 1.0f,
          0.0, 0.0,
          
          1.0, -1.0, 0.0,
          0.0, 0.0, 0.0, 1.0f,
          1.0, 0.0, 
          
          1.0, 1.0, 0.0,
          0.0, 0.0, 0.0, 1.0f,
          1.0, 1.0,
          
          -1.0, 1.0, 0.0,
          0.0, 0.0, 0.0, 1.0f,
          0.0, 1.0,   
        });

        indices.insert(indices.begin(), { 0, 1, 2, 0, 2, 3 });
      }
  };
}