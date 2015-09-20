#pragma once

#include <istream>
#include <vector>

class Shader
{
  private:
    std::vector<char> source;
  
  public:
    Shader(std::istream &input)
    {
      char ch;
      input >> std::noskipws;
      
      while (input >> ch, !input.eof()) {
        source.push_back(ch);
      }
      
      source.push_back('\0');
    }
    
    const char* getSource()
    {
      return &source[0];
    }
};