#pragma once

#include <string>
#include <unordered_map>

class Drawable;

class ShaderProgram
{
  private:
    std::unordered_map<std::string, unsigned> uniformHandles;
    
  public:
    int     gpuHandle;
    
    std::vector<char> vertexShaderSource;
    std::vector<char> fragmentShaderSource;

    ShaderProgram():
      gpuHandle(-1)
    {
    }
    
    unsigned getUniformHandle(const std::string &name) const
    {
      return uniformHandles.at(name);
    }
    
    void saveUniformHandle(const std::string &name, unsigned handle)
    {
      uniformHandles[name] = handle;
    }
    
    void erase()
    {
      vertexShaderSource.clear();
      fragmentShaderSource.clear();
    }

    bool hasGpuHandle() const
    {
      return gpuHandle >= 0;
    }
};