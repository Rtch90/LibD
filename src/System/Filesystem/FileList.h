#pragma once
#include <boost/scoped_ptr.hpp>
#include <vector>
#include <string>
#include "IFileList.h"

namespace saracraft {
namespace filesystem {

class FileList: public IFileList {
struct Data;
boost::scoped_ptr<Data> _data;

public:
  FileList(void);
  ~FileList(void);

  void SetCaseSensitivity(bool enable);

  void AddDir(const std::string& dir);
  void AddFile(const std::string& file);

  int GetDirAmount(const std::string& root) const;
  std::string GetDirName(const std::string& root, int index) const;
  int GetFileAmount(const std::string& root) const;
  const std::string& GetFileName(const std::string& root, int index) const;
};

} // Namespace filesystem.
} // Namespace saracraft.

