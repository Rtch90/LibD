#pragma once
#include <boost/scoped_ptr.hpp>
#include "InputStream.h"

namespace saracraft {
namespace filesystem {

struct InputCompressedFileStreamBufferData;

class InputCompressedFileStreamBuffer : public IInputStreamBuffer {
  boost::scoped_ptr<InputCompressedFileStreamBufferData> _data;
public:
  InputCompressedFileStreamBuffer(const std::string& filename);
  ~InputCompressedFileStreamBuffer(void);

  unsigned char PopByte(void);
  bool IsEof(void) const;
  int GetSize(void) const;

  void PopBytes(char* buffer, int bytes);
};

InputStream CreateInputCompressedFileStream(const std::string& filename);

} // Namespace filesystem.
} // Namespace saracraft.

