#include <vector>
#include <zlib.h>
#include <fstream>

#include "OutputCompressedFileStream.h"

namespace saracraft {
namespace filesystem {

struct OutputCompressedFileStreamBufferData {
  std::ofstream stream;
  std::vector<unsigned char> buffer;

  OutputCompressedFileStreamBufferData(const std::string filename) :
    stream(filename.c_str(), std::ios::binary) {
  
  }
  
  ~OutputCompressedFileStreamBufferData(void) {
  }

  void Compress(void) {
    int bytes = buffer.size();
    stream.write((char*)& bytes, sizeof(int));
    if(!bytes)
      return;

    std::vector<char> tempBuffer(bytes+12);
    Bytef*  source = &buffer[0];
    uLong   sourceLength = buffer.size();
    Bytef*  destination = (Bytef*)& tempBuffer[0];
    uLong   destinationLength = tempBuffer.size();

    int code = ::compress(destination, &destinationLength, source, sourceLength);
    if(code != Z_OK)
      int a = 0;
    stream.write(&tempBuffer[0], destinationLength);
  }
};

OutputCompressedFileStreamBuffer::OutputCompressedFileStreamBuffer(const std::string& filename) {
  boost::scoped_ptr<OutputCompressedFileStreamBufferData> tempData(new OutputCompressedFileStreamBufferData(filename));
  _data.swap(tempData);
}

OutputCompressedFileStreamBuffer::~OutputCompressedFileStreamBuffer(void) {
  _data->Compress();
}

void OutputCompressedFileStreamBuffer::PutByte(unsigned char c) {
  _data->buffer.push_back(c);
  //char c_ = c;
  //_data->stream.Write(&c_, 1);
}

OutputStream CreateOutputCompressedFileStream(const std::string& filename) {
  OutputStream outputStream;
  boost::shared_ptr<OutputCompressedFileStreamBuffer> outputBuffer(new OutputCompressedFileStreamBuffer(filename));

  outputStream.SetBuffer(outputBuffer);
  return outputStream;
}

} // Namespace filesystem.
} // Namespace saracraft.

