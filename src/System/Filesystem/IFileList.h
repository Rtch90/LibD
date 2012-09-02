#pragma once
#include <string>
#include <vector>

namespace saracraft {
namespace filesystem {

class IFileList {
public:
  virtual ~IFileList(void) {}

  virtual void SetCaseSensitivity(bool enable) = 0;

  virtual void AddDir(const std::string& dir) = 0;
  virtual void AddFile(const std::string& file) = 0;

  virtual int GetDirAmount(const std::string& root) const = 0;
  virtual std::string GetDirName(const std::string& root, int index) const = 0;
  virtual int GetFileAmount(const std::string& root) const = 0;
  virtual const std::string& GetFileName(const std::string& root, int index) const = 0;
};

void GetAllFiles(IFileList& list, const std::string& root, std::vector<std::string>& result, bool recurse, bool caseSensitive = false);
void GetAllDirs(IFileList& list, const std::string& root, std::vector<std::string>& result, bool recurse, bool caseSensitive = false);

} // Namespace filesystem.
} // Namespace saracraft.

