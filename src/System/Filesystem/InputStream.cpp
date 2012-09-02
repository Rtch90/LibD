#include  <boost/static_assert.hpp>

#include "../ConvertType.h"
#include <limits.h>
#include "InputStream.h"

#ifdef __INTEL_COMPILER
#pragma warning(disable: 444) // I think intel will complain at the base class not being virtual.
#endif

BOOST_STATIC_ASSERT(sizeof(saracraft::filesystem::uint16_t) * CHAR_BIT == 16);
BOOST_STATIC_ASSERT(CHAR_BIT == 8);

namespace saracraft {
namespace filesystem {
namespace {

  template<class T>
  void ReadFromStream(IInputStreamBuffer& buffer, T& value) {
    ConvertTo<T> converter;
    for(int i = 0; i < converter.GetSize(); ++i)
      converter.SetByte(i, buffer.PopByte());

      value = converter.GetValue();
  }

} // End of unamed namespace.

InputStream::InputStream(void) {

}

InputStream::~InputStream(void) {

}

void InputStream::SetBuffer(boost::shared_ptr<IInputStreamBuffer> streamBuffer_) {
  assert(streamBuffer_);
  _streamBuffer = streamBuffer_;
}

bool InputStream::IsEof(void) const {
  assert(_streamBuffer);
  return _streamBuffer->IsEof();
}

int InputStream::GetSize(void) const {
  assert(_streamBuffer);
  return _streamBuffer->GetSize();
}

InputStream& InputStream::Read(std::string& value) {
  assert(_streamBuffer);

  uint16_t stringSize = 0;
  this->Read(stringSize);

  value.resize(stringSize);
  for(int i = 0; i < stringSize; ++i)
    value[i] = _streamBuffer->PopByte();

  return *this;
}

InputStream& InputStream::Read(bool& value) {
  assert(_streamBuffer);
  value = _streamBuffer->PopByte() != 0;
  return *this;
}

InputStream& InputStream::Read(unsigned char& value) {
  assert(_streamBuffer);

  value = _streamBuffer->PopByte();
  return *this;
}

InputStream& InputStream::Read(char& value) {
  assert(_streamBuffer);

  value = _streamBuffer->PopByte();
  return *this;
}

InputStream& InputStream::Read(signed char& value) {
  assert(_streamBuffer);

  value = _streamBuffer->PopByte();
  return *this;
}

InputStream& InputStream::Read(unsigned short& value) {
  assert(_streamBuffer);

  ReadFromStream(*_streamBuffer, value);
  return *this;
}

InputStream& InputStream::Read(signed short& value) {
  assert(_streamBuffer);

  ReadFromStream(*_streamBuffer, value);
  return *this;
}

InputStream& InputStream::Read(unsigned int& value) {
  assert(_streamBuffer);

  ReadFromStream(*_streamBuffer, value);
  return * this;
}

InputStream& InputStream::Read(signed int& value) {
  assert(_streamBuffer);

  ReadFromStream(*_streamBuffer, value);
  return *this;
}

InputStream& InputStream::Read(float& value) {
  assert(_streamBuffer);

  ReadFromStream(*_streamBuffer, value);
  return *this;
}

InputStream& InputStream::Read(double& value) {
  assert(_streamBuffer);

  ReadFromStream(*_streamBuffer, value);
  return *this;
}

InputStream& InputStream::Read(unsigned char* buffer, int elements) {
  assert(_streamBuffer);

  _streamBuffer->PopBytes(reinterpret_cast<char*>(buffer), elements);
  return *this;
}

InputStream& InputStream::Read(char* buffer, int elements) {
  assert(_streamBuffer);
  _streamBuffer->PopBytes(buffer, elements);

  return *this;
}

InputStream& InputStream::Read(unsigned short* buffer, int elements) {
  assert(_streamBuffer);

  _streamBuffer->PopBytes(reinterpret_cast<char*>(buffer), elements * sizeof(short));
}

} // End of namespace filesystem.
} // End of namespace saracraft.
