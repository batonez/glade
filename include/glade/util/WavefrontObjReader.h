#pragma once

#include <vector>
#include <string>

class DynamicMesh;

class WavefrontObjReader
{
  private:
    static bool putFace(std::string &face, DynamicMesh *mesh);
  public:
    static bool read(const std::vector<char> &in, DynamicMesh *mesh);
};
