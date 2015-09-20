#pragma once

#include <vector>
#include <fstream>
#include "glade/util/Path.h"

class FileManager
{
  private:
    Path basePath;

  public:
    FileManager(const Path &base_path):
      basePath(base_path) {}

    FileManager() {}

    Path getBasePath() const { return basePath; }

    void setBasePath(const Path &base_path) {
      basePath = base_path;
    }

    void setBasePath(const char *base_path) {
      basePath = base_path;
    }

    Path getAbsolutePath(const Path &relative_path) const
    {
      return basePath + relative_path;
    }
    
    void getFileContents(const Path &relative_path, std::ifstream &result, bool binary_mode = false) const;

    template <typename T>
    void getFileContents(const Path &relative_path, std::vector<T> &result, bool binary_mode = false) const;
};