#include <glade/util/AndroidFileManager.h>
#include <glade/log/log.h>
#include <glade/util/Path.h>

#include <android/asset_manager.h>

AndroidFileManager::AndroidFileManager(AAssetManager *android_asset_manager):
  androidAssetManager(android_asset_manager)
{
}

void AndroidFileManager::getFileContents(const Path &relative_path, std::ifstream &result, bool binary_mode) const
{
  log("FileManager::getFilecontents() not implemented for returning ifstream");
}

void AndroidFileManager::getFileContents(const Path &relative_path, std::vector<char> &result, bool binary_mode) const
{
  log("READING.. %s", relative_path.cString());
  AAsset* asset = AAssetManager_open(androidAssetManager, relative_path.cString(), AASSET_MODE_STREAMING);
  uint32_t assetSize = AAsset_getLength(asset);

  if (assetSize > 0) {
    result.resize(assetSize);
    AAsset_read(asset, result.data(), assetSize);
  }

  AAsset_close(asset);
}

void DesktopFileManager::getFileContents(const Path &relative_path, std::vector<unsigned char> &result, bool binary_mode) const
{
  std::ifstream input;
  getFileContents(relative_path, input, binary_mode);
  assert(input.good());

  unsigned char val;

  if (binary_mode) {
    while (!input.eof()) {
      input.read((unsigned char *) &val, sizeof(unsigned char));
      result.push_back(val);
    }
  } else {
    input >> std::noskipws;

    while (input >> val, !input.eof()) {
      result.push_back(val);
    }
  }
}