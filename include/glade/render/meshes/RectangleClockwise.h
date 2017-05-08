#pragma once

#include "Mesh.h"

namespace Glade {
  class RectangleClockwise: public Mesh
  {
    public:
      RectangleClockwise() : Mesh() {
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

        indices.insert(indices.begin(), { 2, 1, 0, 3, 2, 0 });
      }
  };
}