#pragma once

#include "Mesh.h"

namespace Glade {
  class Triangle : public Mesh
  {
    public:
      Triangle() : Mesh() {
        vertices.insert(vertices.begin(), {
          0.0f, 0.5f, 0.0f,  // position
          0.0f, 0.0f, 0.0f, 1.0f, // normal (the last number indicates whether the whole vertex is transparent or not)
          0.0f, 0.0f, // texcoord

          -0.5f, -0.5f, 0.0f,
          0.0f, 0.0f, 0.0f, 1.0f,
          0.0f, 1.0f,

          0.5f, -0.5f, 0.0f,
          0.0f, 0.0f, 0.0f, 1.0f,
          1.0f, 1.0f
        });

        indices.insert(indices.begin(), { 0, 1, 2 });
      }
  };
}