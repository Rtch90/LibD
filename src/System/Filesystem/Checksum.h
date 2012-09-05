#pragma once

namespace saracraft {
namespace filesystem {

class Checksum {
public:
  static unsigned int CountChecksumForFile(const char* filename);
  static bool DoesChecksumAndSizeMatchFile(unsigned int checksum, 
                                      int filesize, const char* filename);
private:
  static bool CountChecksumForFileImpl(unsigned int* checksum, 
                                      int* filesize, const char* filename);
};

} // Namespace filesystem.
} // Namespace saracraft.
