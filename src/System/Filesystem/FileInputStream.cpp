#include <fstream>
#include "../Debug.h"
#include "InputFileStream.h"

namespace saracraft {
namespace filesystem {

struct InputFileStreamBufferData {
  FILE* fp;
  size_t size;

  InputFileStreamBufferData(const std::string filename) : fp(0), size(0) {
    fp = fopen(filename.c_str(), "rb");

#ifdef __UNIX__
    // Another ugly as hell case sensitive hack.
    // if anyone finds a nice way to handle this. Please implement it. -- Allanis
    if(!fp) {
      std::string filename = filename;
      for(unsigned int i = 0; i < filename.size(); i++) {
        if(isupper(filename[i]))
          filename[i] = tolower(filename[i]);
        else if(filename[i] == '\\')
          filename[i] = '/';
      }
      fp = fopen(filename.c_str(), "rb");
    }
#endif
    if(fp) {
      fseek(fp, 0, SEEK_END);
      size = ftell(fp);
      fseek(fp, 0, SEEK_SET);
    }
    if(!size)
      Close();
  }

  ~InputFileStreamBufferData(void) {
    Close();
  }

  void Close(void) {
    if(fp) {
      fclose(fp);
      fp = 0;
    }
  }
};

InputFileStreamBuffer::~InputFileStreamBuffer(void) {

}

unsigned char InputFileStreamBuffer::PopByte(void) {
  char byte = 0;
  if(_data->fp) {
    int input = fgetc(_data->fp);
    if(input == EOF)
      _data->Close();
    else
      byte = (char)input;
  }
  return byte;
}

bool InputFileStreamBuffer::IsEof(void) const {
  if(!_data->fp)
    return true;

  return false;
}

int InputFileStreamBuffer::GetSize(void) const {
  if(!_data->fp)
    return 0;

  // This could cause maybe about a 2GB limimitation to file size
  // by casting to int. Feel free to FIXME.
  // -- Allanis
  return int(_data->size);
}

void InputFileStreamBuffer::PopBytes(char* buffer, int bytes) {
  if(!_data->fp) {
    for(int i = 0; i < bytes; ++i)
      buffer[i] = 0;
  } else {
    //_data->stream.read(buffer, bytes);
    if(fread(buffer, 1, bytes, _data->fp) != (unsigned)bytes)
      _data->Close();
  }
}

// HACK: ffs. This is needed to get some sense into the input stream error reporting.
//bool input_file_stream_no_nonexisting_error_message = false;

//void SetInputStreamErrorReporting(bool logNonExisting) {
//  input_file_stream_no_nonexisting_error_message = !logNonExisting;
//}

InputStream CreateInputFileStream(const std::string& filename) {

  InputStream inputStream;
  boost::shared_ptr<InputFileStreamBuffer> inputBuffer(new InputBuffer(filename));
  // Eh.. No.
  // TODO: Would need a seperate error check, eof is not the same as the file does not exist..
  // This would just spam error messages. We don't want that.
  /*if(!inputBuffer->IsEof()) {
    if(!input_file_stream_no_nonexisting_error_message) {
      Debug->logger("CreateInputFileStream - File \"%s\"does not exist or is zero length.", filename.c_str());
    }
  }*/
  inputStream.SetBuffer(inputBuffer);
  return inputStream;
}

} // Namespace filesystem.
} // Namespace saracraft.

