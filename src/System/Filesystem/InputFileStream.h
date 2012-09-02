// I forgot some code..
#pragma once
#include <boost/scoped_ptr.hpp>
#include "InputStream.h"

namespace saracraft {
namespace filesystem {

struct InputFileStreamBufferData;

class InputFileStreamBuffer: public IInputStreamBuffer {
  boost::scoped_ptr<InputFileStreamBufferData> _data;

public:
  InputFileStreamBuffer(const std::string& filename);
  ~InputFileStreamBuffer(void);

  unsigned char PopByte(void);
  bool IsEof(void) const;
  int GetSize(void) const;

  void PopBytes(char* buffer, int bytes);
};

InputStream CreateInputFileStream(const std::string& filename);
void SetInputStreamErrorReporting(bool logNonExisting);

} // Namespace filesystem.
} // Namespace saracraft.

