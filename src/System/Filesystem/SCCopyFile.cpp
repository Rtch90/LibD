#include <fstream>
#include <string>
#include "InputStream.h"
#include "FilePackageManager.h"

#include "SCCopyFile.h"

namespace util {

void SCCopyFile::CopyFile(const std::string& from, const std::string& to) {
  std::fstream out(to.c_str(), std::ios::out);
  saracraft::filesystem::InputStream in = saracraft::filesystem::FilePackageManager::GetInstance().GetFile(from);

  std::string temp;
  temp.resize(in.GetSize());
  in.Read((unsigned char*)&temp[0], in.GetSize());
  out << temp << std::endl;

  out.close();
}

} // Namespace util.

