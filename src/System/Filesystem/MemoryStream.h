#include <boost/scoped_ptr.hpp>
#include "InputStream.h"
#include "OutputStream.h"

namespace saracraft {
namespace filesystem {

struct MemoryStreamBufferData;

class MemoryStreamBuffer : public IInputStreamBuffer, public IOutputStreamBuffer {
  boost::scoped_ptr<MemoryStreamBufferData> _data;
public:
  MemoryStreamBuffer(void);
  ~MemoryStreamBuffer(void);

  unsigned char PopByte(void);
  bool IsEof(void) const;
  int GetSize(void) const;

  void PutByte(unsigned char byte);
  void PopBytes(char* buffer, int bytes);
};

} // Namespace filesystem.
} // Namespace saracraft.

