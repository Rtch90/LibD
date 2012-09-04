#include <vector>

#include "../Debug.h"
#include "InputStreamWrapper.h"
#include "FilePackageManager.h"

namespace saracraft { 
namespace filesystem {
namespace {
  int openFileAmount = 0;

  struct Tracker {
    Tracker(void) {}

    ~Tracker(void) { assert(openFileAmount == 0); }
  };

  Tracker tracker;
} // Namespace Unamed.

struct SC_FILE {
  InputStream stream;
    
  SC_FILE(InputStream& stream_) : stream(stream_) {
    ++openFileAmount;
  }

  ~SC_FILE(void) {
    --openFileAmount;
  }
};

SC_FILE* sc_fopen(const char* filename, const char*) {
  if(!filename)
    return 0;

  FilePackageManager& manager = FilePackageManager::GetInstance();
  manager.SetInputStreamErrorReporting(false);
  InputStream stream = manager.GetFile(filename);
  manager.SetInputStreamErrorReporting(true);

  if(stream.IsEof())
    return 0;
  return new SC_FILE(stream);
}

size_t sc_fread(void* buffer, size_t size, size_t count, SC_FILE* stream) {
  if(!stream) {
    Debug::logger->message("sc_fread - Attempt to read when no stream is available.");
    return 0;
  }

  if(stream->stream.IsEof()) {
    Debug::logger->message("sc_fread - Attempt to read past the end of the file.");
  }

  unsigned char* charBuffer = reinterpret_cast<unsigned char*> (buffer);
  stream->stream.Read(charBuffer, size * count);

  return count;
}

size_t sc_fsize(SC_FILE* stream) {
  if(!stream)
    return 0;

  return stream->stream.GetSize();
}

int sc_feof(SC_FILE* stream) {
  if(!stream || stream->stream.IsEof())
    return 1;
  return 0;
}

int sc_fclose(SC_FILE* stream) {
  delete stream;
  return 0;
}

} // Namespace filesystem.
} // Namespace saracraft.

