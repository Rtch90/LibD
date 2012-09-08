#pragma once
#include <boost/scoped_ptr.hpp>
#include "OutputStream.h"

namespace saracraft {
namespace filesystem {

struct OutputCompressedFileStreamBufferData;

class OutputCompressedFileStreamBuffer : public IOutputStreamBuffer {
  boost::scoped_ptr<OutputCompressedFileStreamBufferData> _data;
public: 
  OutputCompressedFileStreamBuffer(const std::string& filename);
  ~OutputCompressedFileStreamBuffer(void);

  void PutByte(unsigned char c);
};

OutputStream CreateOutputCompressedFileStream(const std::string& filename);

} // Namespace filesystem.
} // Namespace saracraft.

