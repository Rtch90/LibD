#pragma once

namespace saracraft {
namespace filesystem {

class SCCopyFile {
public:

  // TODO: This will currently only handle smallish text files
  // propperly. 

  /*** DON'T USE FOR BINARY FILES!!!! ***/
  static void CopyFile(const std::string& from, const std::string& to);
};

} // Namespace filesystem.
} // Namespace saracraft.
