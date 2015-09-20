#include <glade/util/FileManager.h>
#include <glade/exception/GladeException.h>
#include <glade/log/log.h>

#include <string>
#include <fstream>
#include <assert.h>

void FileManager::getFileContents(const Path &relative_path, std::ifstream &result, bool binary_mode) const
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

template <typename T>
void FileManager::getFileContents(const Path &relative_path, std::vector<T> &result) const
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