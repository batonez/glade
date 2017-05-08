#include "Mesh.h"

namespace Glade {
  class Square: public Mesh
  {
    public:
      Square(): Mesh() {
        vertices.insert(vertices.begin(), {
          0.5f, 0.5f, 0.0f,
          0.577349f, 0.577349f, -0.577349f, 1.0f,
          0.000000f, 0.000000f,

          0.5f, -0.5f, 0.0f,
          0.577349f, -0.577349f, -0.577349f, 1.0f,
          0.000000f, 1.000000f,

          -0.5f, -0.5f, 0.0f,
          -0.577349f, -0.577349f, -0.577349f, 1.0f,
          1.000000f, 0.000000f,

          -0.5f, 0.5f, 0.0f,
          -0.577349f, 0.577349f, -0.577349f, 1.0f,
          1.000000f, 1.000000f,
        });

        indices.insert(indices.begin(), {
          2, 1, 0,
          2, 0, 3,
        });
      }
  };
}