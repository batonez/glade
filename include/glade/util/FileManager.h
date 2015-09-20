#pragma once

#include <string>
#include <fstream>

#include "glade/exception/GladeException.h"
#include "glade/util/Path.h"
#include "glade/log/log.h"

class FileManager
{
  private:
    Path basePath;

  public:
    FileManager(const Path &base_path):
      basePath(base_path)
    {}
    
    FileManager() {}
    
    Path getBasePath() const { return basePath; }
    
    void setBasePath(const Path &base_path) {
      basePath = base_path;
    }
    
    void setBasePath(const char *base_path) {
      basePath = base_path;
    }
    
    void getFileContents(const Path &relative_path, std::ifstream &result, bool binary_mode = false) const
    {
      if (result.is_open()) {
        log("Warning: FileManager::getFileContents: provided input stream object is open. File manager will close it");
        result.close();
      }
      
      Path absolutePath = getAbsolutePath(relative_path);
      
      if (binary_mode) {
        result.open(absolutePath.cString(), std::ifstream::in | std::ifstream::binary);
      } else {
        result.open(absolutePath.cString(), std::ifstream::in);
      }
      
      if (!result.good()) {
        // TODO make GladeException to recieve varargs just like log
        log("Error: FileManager::getFileContents: Could not open file %s", absolutePath.cString());
        throw GladeException("Error: FileManager::getFileContents: Could not open file");
      }
    }
    
    template <typename T = char>
    void getFileContents(const Path &relative_path, std::vector<T> &result, bool binary_mode = false) const
    {
      std::ifstream input;
      getFileContents(relative_path, input, binary_mode);
      assert(input.good());
      
      T val;
      
      if (binary_mode) {
        while (!input.eof()) {
          input.read((char *) &val, sizeof(T));
          result.push_back(val);
        }
      } else {
        input >> std::noskipws;
        
        while (input >> val, !input.eof()) {
          result.push_back(val);
        }
      }
    }
    
    Path getAbsolutePath(const Path &relative_path) const
    {
      return basePath + relative_path;
    }
};