#pragma once

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "IFilePackage.h"

namespace saracraft {
namespace filesystem {

class IFileList;
struct FilePackageManagerData;

class FilePackageManager {
  boost::scoped_ptr<FilePackageManagerData> _data;
public:
  FilePackageManager(void);
  ~FilePackageManager(void);

  void AddPackage(boost::shared_ptr<IFilePackage> filePackae, int priority);
  boost::shared_ptr<IFileList> FindFiles(const std::string& dir, const std::string& extension, bool caseSensitive = false);
  InputStream GetFile(const std::string& filename);

  void SetInputStreamErrorReporting(bool logNonExisting);

  static FilePackageManager& GetInstance(void);
  static void SetInstancePtr(FilePackageManager* instance);
};

} // Namespace filesystem.
} // Namespace saracraft.

