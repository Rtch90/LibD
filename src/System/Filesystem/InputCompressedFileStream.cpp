#include <fstream>
#include <vector>
#include <zlib.h>

#include "../Debug.h"
#include "FileTimestampChecker.h"
#include "InputCompressedFileStream.h"

namespace saracraft {
namespace filesystem {

struct InputCompressedFileStreamBufferData {
  std::vector<unsigned char> buffer;
  int readPosition;
  int timeValue;

  InputCompressedFileStreamBufferData(const std::string filename) : readPosition(0), timeValue(0) {

    std::ifstream stream(filename.c_str(), std::ios::binary);
    if(!stream)
      return;

    /*std::filebuf* streamBuffer = stream.rdbuf();
    if(!streamBuffer)
      return;
    std::streamsize compressedSize = streamBuffer->in_avail() - sizeof(int); */

    stream.seekg(0, std::ios::end);
    std::streamsize compressedSize = stream.tellg();
    compressedSize -= sizeof(int);
    stream.seekg(0, std::ios::beg);

    int bytes = 0;
    stream.read((char*) &bytes, sizeof(int));
    if(!bytes)
      return;
    
    buffer.resize(bytes);
    std::vector<char> compressedBuffer((int)compressedSize);
    stream.read(&compressedBuffer[0], compressedSize);

    Bytef* source = (Bytef*)&compressedBuffer[0];
    uLong sourceLength = compressedBuffer.size();
    Bytef* destination = &buffer[0];
    uLong destinationLength = buffer.size();

    int code = ::uncompress(destination, &destinationLength, source, sourceLength);
    (void)code;
    //if(code != Z_OK)
    //  int a = 0;

    timeValue = FileTimestampChecker::GetFileTimestamp(filename.c_str());
  }
};

InputCompressedFileStreamBuffer::InputCompressedFileStreamBuffer(const std::string& filename) {
  boost::scoped_ptr<InputCompressedFileStreamBufferData> tempData(new InputCompressedFileStreamBufferData(filename));
  _data.swap(tempData);
}

InputCompressedFileStreamBuffer::~InputCompressedFileStreamBuffer(void) {

}

unsigned char InputCompressedFileStreamBuffer::PopByte(void) {
  char byte = 0;
  if(!IsEof())
    byte = _data->buffer[_data->readPosition++];
  return byte;
}

bool InputCompressedFileStreamBuffer::IsEof(void) const {
  if(_data->readPosition >= GetSize())
    return true;
  return false;
}

int InputCompressedFileStreamBuffer::GetSize(void) const {
  return int(_data->buffer.size());
}

void InputCompressedFileStreamBuffer::PopBytes(char* buffer, int bytes) {
  int readSize = bytes;
  
  if(_data->readPosition + readSize > GetSize()) {
    readSize = GetSize() - _data->readPosition;
    for(int i = readSize; i < bytes; ++i)
      buffer[i] = 0;
  }

  for(int i = 0; i < readSize; ++i)
    buffer[i] = _data->buffer[_data->readPosition++];
}

InputStream CreateInputCompressedFileStream(const std::string& filename) {
  InputStream inputStream;
  boost::shared_ptr<InputCompressedFileStreamBuffer> inputBuffer(new InputCompressedFileStreamBuffer(filename));

  inputStream.SetBuffer(inputBuffer);
  return inputStream;
}

} // Namespace filesystem.
} // Namespace saracraft.

