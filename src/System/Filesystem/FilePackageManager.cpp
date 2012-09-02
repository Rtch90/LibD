#include <map>
#include "../Debug.h"
#include "FilePackageManager.h"
#include "EmptyBuffer.h"
#include "FileList.h"

// HACK: This is not a good dependancy for error reporting.
// We are going to use it anyway though.
#include "InputFileStream.h"

namespace saracraft {
namespace filesystem {
namespace {
typedef std::multimap<int, boost::shared_ptr<IFilePackage> > PackageMap;
  FilePackageManager instance;
  FilePackageManager* instancePtr = 0;
} // Unamed namespace.

struct FilePackageManagerData {
  PackageMap packages;
  bool logNonExisting;

  FilePackageManagerData(void) : logNonExisting(true) {}

  InputStream GetFile(std::string filename) {
    for(unsigned int i = 0; i < filename.size(); ++i) {
      if(filename[i] == '\\')
        filename[i] = '/';
    }

    for(PackageMap::reverse_iterator it = packages.rbegin(); it != packages.rend(); ++it) {
      InputStream result = it->second->GetFile(filename);
      if(!result.IsEof())
        return result;
    }

    // This is a bit of a hack.
    // Fix all data to use only lowercase letters if you
    // want it to work right. But you know me and upercase characters. --Allanis
    for(unsigned int i = 0; i < filename.size(); ++i) {
      // Not found try again in lowercase..
      if(isupper(filename[i]))
        filename[i] = tolower(filename[i]);
    }

    // Nothing has been found.
    if(logNonExisting) {
      Debug::logger->message("FilePackageManager::GetFile - File: \"%s\" does not exist or is zero in length!", filename.c_str());
    }
    InputStream inputStream;
    boost::shared_ptr<EmptyBuffer> inputBuffer(new EmptyBuffer());

    inputStream.SetBuffer(inputBuffer);
    return inputStream;
  }
};

FilePackageManager::FilePackageManager(void) {
  boost::scoped_ptr<FilePackageManagerData> tempData(new FilePackageManagerData());
  _data.swap(tempData);
}

FilePackageManager::~FilePackageManager(void) {
}

void FilePackageManager::AddPackage(boost::shared_ptr<IFilePackage> filePackage, int priority) {
  std::pair<int, boost::shared_ptr<IFilePackage> > value(priority, filePackage);
  _data->packages.insert(value);
}

boost::shared_ptr<IFileList> FilePackageManager::FindFiles(const std::string& dir, const std::string& extension, bool caseSensitive) {
  boost::shared_ptr<IFileList> result(new FileList());
  result->SetCaseSensitivity(caseSensitive);

  for(PackageMap::iterator it = _data->packages.begin(); it != _data->packages.end(); ++it)
    it->second->FindFiles(dir, extension, *result);
  
  return result;
}

InputStream FilePackageManager::GetFile(const std::string& filename) {
  return _data->GetFile(filename);
}

void FilePackageManager::SetInputStreamErrorReporting(bool logNonExisting) {
  // HACK: This goes directly to the input file stream...
  _data->logNonExisting = logNonExisting;
}

FilePackageManager& FilePackageManager::GetInstance(void) {
  if(!instancePtr)
    instancePtr = &instance;
  return *instancePtr;
}

void FilePackageManager::SetInstancePtr(FilePackageManager* newInstance) {
  assert(newInstance);
  instancePtr = newInstance;

  //FilePackageManager& oldInstance = GetInstance();
  //oldInstance._data->packages = newInstance->_data->packages;
}

} // Namespace filesystem.
} // Namespace saracraft.

