#include <queue>
#include "MemoryStream.h"

namespace saracraft {
namespace filesystem {

struct MemoryStreamBufferData {
  std::queue<unsigned char> buffer;
};

MemoryStreamBuffer::MemoryStreamBuffer(void) {
  boost::scoped_ptr<MemoryStreamBufferData> tempData(new MemoryStreamBufferData());
  _data.swap(tempData);
}

MemoryStreamBuffer::~MemoryStreamBuffer(void) {
  
}

unsigned char MemoryStreamBuffer::PopByte(void) {
  if(!_data->buffer.empty()) {
    unsigned char value = _data->buffer.front();
    _data->buffer.pop();
    return value;
  }
  return 0;
}

bool MemoryStreamBuffer::IsEof(void) const {
  return _data->buffer.empty();
}

int MemoryStreamBuffer::GetSize(void) const { 
  return _data->buffer.size();
}

void MemoryStreamBuffer::PutByte(unsigned char byte) {
  _data->buffer.push(byte);
}

void MemoryStreamBuffer::PopBytes(char* buffer, int bytes) {
  for(int i = 0; i < bytes; ++i)
    buffer[i] = PopByte();
}

} // Namespace filesystem.
} // Namespace saracraft.

