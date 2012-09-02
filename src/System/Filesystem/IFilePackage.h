#pragma once
#include "InputStream.h"

namespace saracraft {
namespace filesystem {

class IFileList;
class IFilePackage {
public:
  virtual ~IFilePackage(void) {}

  virtual void FindFiles(const std::string& dir, const std::string& extension, IFileList& result) = 0;
  virtual InputStream GetFile(const std::string& filename) = 0;
};

} // Namespace filesystem.
} // Namespace saracraft.

