#pragma once

#include "InputStream.h"

namespace saracraft {
namespace filesystem {

struct EmptyBuffer: public IINputStreamBuffer {
  unsigned char PopByte(void) { return 0; }
  bool IsEof(void) const      { return true; }
  int GetSize(void) const     { return 0; }
  void PopBytes(char*, int)   { }
};

} // Namespace filesystem.
} // Namespace saracraft.

