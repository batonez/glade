#pragma once

#include <istream>

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
    }
    
    const char* getSource()
    {
      return source.data();
    }
};