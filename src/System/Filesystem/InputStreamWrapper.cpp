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

struct FB_FILE {
  InputStream stream;
    
  FB_FILE(InputStream& stream_) : stream(stream_) {
    ++openFileAmount;
  }

  ~FB_FILE(void) {
    --openFileAmount;
  }
};

FB_FILE* fb_fopen(const char* filename, const char*) {
  if(!filename)
    return 0;

  FilePackageManager& manager = FilePackageManager::GetInstance();
  manager.SetInputStreamErrorReporting(false);
  InputStream stream = manager.GetFile(filename);
  manager.SetInputStreamErrorReporting(true);

  if(stream.IsEof())
    return 0;
  return new FB_FILE(stream);
}

size_t fb_fread(void* buffer, size_t size, size_t count, FB_FILE* stream) {
  if(!stream) {
    Debug::logger->message("fb_fread - Attempt to read when no stream is available.");
    return 0;
  }

  if(stream->stream.IsEof()) {
    Debug::logger->message("fb_fread - Attempt to read past the end of the file.");
  }

  unsigned char* charBuffer = reinterpret_cast<unsigned char*> (buffer);
  stream->stream.Read(charBuffer, size * count);

  return count;
}

size_t fb_fsize(FB_FILE* stream) {
  if(!stream)
    return 0;

  return stream->stream.GetSize();
}

int fb_feof(FB_FILE* stream) {
  if(!stream || stream->stream.IsEof())
    return 1;
  return 0;
}

int fb_fclose(FB_FILE* stream) {
  delete stream;
  return 0;
}

} // Namespace filesystem.
} // Namespace saracraft.

