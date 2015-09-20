#include <glade/util/FileManager.h>
#include <glade/log/log.h>

#include <android/asset_manager.h>

/*
void FileManager::getFileContents(const Path &relative_path, std::ifstream &result, bool binary_mode) const
{
  log("FileManager::getFilecontents() not implemented for returning ifstream");
}
*/

template <typename T>
void FileManager::getFileContents(const Path &relative_path, std::vector<T> &result, bool binary_mode) const
{
  AAsset* asset = AAssetManager_open(AAssetManager* mgr, const char* filename, int mode);
}