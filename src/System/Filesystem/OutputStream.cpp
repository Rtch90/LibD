#include  <boost/static_assert.hpp>

#include "../Convert/ConvertType.h"
#include <limits.h>
#include "OutputStream.h"

#ifdef __INTEL_COMPILER
#pragma warning(disable: 444) // I think intel will complain at the base class not being virtual.
#endif

BOOST_STATIC_ASSERT(sizeof(saracraft::filesystem::uint16_t) * CHAR_BIT == 16);
BOOST_STATIC_ASSERT(CHAR_BIT == 8);

namespace saracraft {
namespace filesystem {
namespace {

  template<class T>
  void SendToStream(IOutputStreamBuffer& buffer, T& value) {
    ConvertFrom<T> converter(value);
    for(int i = 0; i < converter.GetSize(); ++i)
      buffer.PutByte(converter.GetByte(i));
  }

} // End of unamed namespace.

OutputStream::OutputStream(void) : _textStrings(false){

}

OutputStream::~OutputStream(void) {

}

void OutputStream::SetBuffer(boost::shared_ptr<IOutputStreamBuffer> streamBuffer_) {
  assert(streamBuffer_);
  _streamBuffer = streamBuffer_;
}

void OutputStream::UseTextStrings(void) {
 _textStrings = true;
}


OutputStream& OutputStream::Write(std::string& value) {
  assert(_streamBuffer);

  uint16_t stringSize = 0;
  
  if(!_textStrings)
		Write(stringSize);

  for(int i = 0; i < stringSize; ++i)
    _streamBuffer->PutByte(value[i]);

  return *this;
}

OutputStream& OutputStream::Write(bool& value) {
  assert(_streamBuffer);
  unsigned char b = (value) ? 1:0;
  _streamBuffer->PutByte(b);
  return *this;
}

OutputStream& OutputStream::Write(unsigned char& value) {
  assert(_streamBuffer);

  _streamBuffer->PutByte(value);
  return *this;
}

OutputStream& OutputStream::Write(char& value) {
  assert(_streamBuffer);

  _streamBuffer->PutByte(value);
  return *this;
}

OutputStream& OutputStream::Write(signed char& value) {
  assert(_streamBuffer);

  _streamBuffer->PutByte(value);
  return *this;
}

OutputStream& OutputStream::Write(unsigned short& value) {
  assert(_streamBuffer);

  SendToStream(*_streamBuffer, value);
  return *this;
}

OutputStream& OutputStream::Write(signed short& value) {
  assert(_streamBuffer);

  SendToStream(*_streamBuffer, value);
  return *this;
}

OutputStream& OutputStream::Write(unsigned int& value) {
  assert(_streamBuffer);

  SendToStream(*_streamBuffer, value);
  return * this;
}

OutputStream& OutputStream::Write(signed int& value) {
  assert(_streamBuffer);

  SendToStream(*_streamBuffer, value);
  return *this;
}

OutputStream& OutputStream::Write(float& value) {
  assert(_streamBuffer);

  SendToStream(*_streamBuffer, value);
  return *this;
}

OutputStream& OutputStream::Write(double& value) {
  assert(_streamBuffer);

  SendToStream(*_streamBuffer, value);
  return *this;
}

} // End of namespace filesystem.
} // End of namespace saracraft.
