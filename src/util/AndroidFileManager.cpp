#include <glade/util/AndroidFileManager.h>
#include <glade/debug/log.h>
#include <glade/util/Path.h>
#include <glade/exception/GladeFileNotFoundException.h>

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
  AAsset* asset = AAssetManager_open(androidAssetManager, relative_path.cString(), AASSET_MODE_STREAMING);

  if (asset == NULL) {
    throw GladeFileNotFoundException("File not found");
  }

  uint32_t assetSize = AAsset_getLength(asset);

  if (assetSize > 0) {
    result.resize(assetSize);
    AAsset_read(asset, result.data(), assetSize);
  }

  AAsset_close(asset);
}

void AndroidFileManager::getFileContents(const Path &relative_path, std::vector<unsigned char> &result, bool binary_mode) const
{
  AAsset* asset = AAssetManager_open(androidAssetManager, relative_path.cString(), AASSET_MODE_STREAMING);
  
  if (asset == NULL) {
    throw GladeFileNotFoundException("File not found");
  }

  uint32_t assetSize = AAsset_getLength(asset);

  if (assetSize > 0) {
    result.resize(assetSize);
    AAsset_read(asset, result.data(), assetSize);
  }

  AAsset_close(asset);
}
