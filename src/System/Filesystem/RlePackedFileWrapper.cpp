#include <stdio.h>
#include <assert.h>

#include "SCpak.h"
#include "InputStreamWrapper.h"
#include "RlePackedFileWrapper.h"

#define USE_SC_INPUT_STREAM_WRAPPER

static const int RLE_PACKED_CHUNK_MAGIC = *((int*)"RLE1");

using namespace saracraft::filesystem;


struct RLE_PACKED_FILE {
#ifdef USE_SC_INPUT_STREAM_WRAPPER
  SC_FILE* inputFile;
#else
  FILE* inputtFile;
#endif
  FILE* outputFile;
  bool errorFlag;
  bool inputIsRle;

  RLE_PACKED_FILE(void) :
    inputFile(NULL), outputFile(NULL), errorFlag(false), inputIsRle(false)  {
  
  }

  ~RLE_PACKED_FILE(void) {
    assert(inputFile == NULL);
    assert(outputFile == NULL);
  }
};

bool RlePackedDetect(const char* filename) {
  bool wasRLE = false;
#ifdef USE_SC_INPUT_STREAM_WRAPPER
  SC_FILE* inputFile = sc_fopen(filename, "rb");
#else
  FILE* inputFile = fopen(filename, "rb");
#endif
  if(inputFile != NULL) {
    int chunkId;
#ifdef USE_SC_INPUT_STREAM_WRAPPER
    int got = sc_fread(&chunkId, sizeof(int), 1, inputFile);
#else
    int got = fread(&chunkId, sizeof(int), 1, inputFile);
#endif
    if(got == 1 && chunkId == RLE_PACKED_CHUNK_MAGIC) {
      wasRLE = true;
    }

#ifdef USE_SC_INPUT_STREAM_WRAPPER
    sc_fclose(inputFile);
#else
    fclose(inputFile);
#endif
  }
  return wasRLE;
}

RLE_PACKED_FILE* RlePacked_fopen(const char* filename, const char* params) {
  assert(params != NULL);
  if(params == NULL)
    return NULL;

  if(strcmp(params, "rb") == 0) {
    bool rle = false;
    if(RlePackedDetect(filename))
      rle = true;

#ifdef USE_SC_INPUT_STREAM_WRAPPER
    SC_FILE* inputFile = sc_fopen(filename, params);
#else
    FILE* inputFile = fopen(filename, params);
#endif
    if(inputFile != NULL) {
      RLE_PACKED_FILE* f = new RLE_PACKED_FILE();
      f->inputFile = inputFile;
      f->inputIsRle = rle;
      return f;
    } else {
      return NULL;
    }
  }
  else if(strcmp(params, "wb") == 0) {
    FILE* outputFile = fopen(filename, params);
    if(outputFile != NULL) {
      RLE_PACKED_FILE* f = new RLE_PACKED_FILE();
      f->outputFile = outputFile;
      return f;
    } else {
      return NULL;
    }
  }
  assert(!"RlePacked_fopen - unsupported open params.");
  return NULL;
}

size_t RlePacked_fread(void* buffer, size_t size, size_t count, RLE_PACKED_FILE* stream) {
  assert(stream != NULL);
  assert(stream->inputFile != NULL);

  if(!stream->inputIsRle) {
#ifdef USE_SC_INPUT_STREAM_WRAPPER
    size_t ret = sc_fread(buffer, size, count, stream->inputFile);
#else
    size_t ret = fread(buffer, size, count, stream->inputFile);
#endif
    if(ret <= 0)
      stream->errorFlag = true;
    return ret;
  }

  int   tmpgot        = 0;
  int   chunkId       = 0;
  char  isPacked      = 0;
  int   packedSize    = 0;
  int   unpackedSize  = 0;

#ifdef USE_SC_INPUT_STREAM_WRAPPER
  tmpgot = sc_fread(&chunkId, sizeof(int), 1, stream->inputFile);
#else
  tmpgot = fread(&chunkId, sizeof(int), 1, stream->inputFile);
#endif
  if(tmpgot != 1) {
    assert(!"RlePacked_fread - no more data available (expected chunk id, but got nothing).");
    stream->errorFlag = true;
    return 0;
  }
  if(chunkId != RLE_PACKED_CHUNK_MAGIC) {
    assert(!"RlePacked_fread - invalid data (expected chunk id, but got something else).");
    stream->errorFlag = true;
    return 0;
  }
#ifdef USE_SC_INPUT_STREAM_WRAPPER
  tmpgot = sc_fread(&isPacked, sizeof(char), 1, stream->inputFile);
#else
  tmpgot = fread(&isPacked, sizor(char), 1, stream->inputFile);
#endif
  if(tmpgot != 1) {
    assert(!"RlePacked_fread - truncated chunk header (expected packed flag). ");
    stream->errorFlag = true;
    return 0;
  }
  if(isPacked != 0 && isPacked != 1) {
    assert(!"RlePacked_fread - invalid data (expected packed flag, but got something else). ");
    stream->errorFlag = true;
    return 0;
  }
#ifdef USE_SC_INPUT_STREAM_WRAPPER
  tmpgot = sc_fread(&packedSize, sizeof(int), 1, stream->inputFile);
#else
  tmpgot = fread(&packedSize, sizeof(int), 1, stream->inputFile);
#endif
  if(tmpgot != 1) {
    assert(!"RlePacked_fread - truncated chunk header (expected packed size). ");
    stream->errorFlag = true;
    return 0;
  }
  if(packedSize <= 0) {
    assert(!"RlePacked_fread - invalid data (packed size is zero or negative). ");
    stream->errorFlag = true;
    return 0;
  }
  if(packedSize > (int)count * (int)size) {
    assert(!"RlePacked_fread - invalid data (size of packed chunk is larger than requested data). ");
    stream->errorFlag = true;
    return 0;
  }
#ifdef USE_SC_INPUT_STREAM_WRAPPER
  tmpgot = sc_fread(&unpackedSize, sizeof(int), 1, stream->inputFile);
#else
  tmpgot = fread(&unpackedSize, sizeof(int), 1, stream->inputFile);
#endif
  if(tmpgot != 1) {
    assert(!"RlePacked_fread - truncated chunk header (expected unpacked size). ");
    stream->errorFlag = true;
    return 0;
  }
  if(unpackedSize <= 0) {
    assert(!"RlePacked_fread - invalid data (unpacked size is zero or negative). ");
    stream->errorFlag = true;
    return 0;
  }
  if(unpackedSize != count * size) {
    assert(!"RlePacked_fread - invalid data (size of unpacked chunk does not equal requested size).");
    stream->errorFlag = true;
    return 0;
  }
  if(!isPacked && packedSize != unpackedSize) {
    assert(!"RlePacked_fread - invalid data (supposed to be unpacked chunk, but packed and unpacked size mismatch).");
    stream->errorFlag = true;
    return 0;
  }

  unsigned char* packedbuf = new unsigned char[packedSize+16];

#ifdef USE_SC_INPUT_STREAM_WRAPPER
  int got = sc_fread(packedbuf, packedSize, 1, stream->inputFile);
#else
  int got = fread(packedbuf, packedSize, 1, stream->inputFile);
#endif
  if(got == 1) {
    if(isPacked) {
      scpak_set_bits(16);
      scpak_set_16bit_params(0xA0EA, 0xA0EB, 0xA0EC);
      int resultSize = scpak_unpack(packedSize, packedbuf, (unsigned char*)buffer, unpackedSize);
      if(resultSize != unpackedSize) {
        assert(!"RlePacked_fread - invalid data (chunk header unpacked size did not match actual resulting unpacked size).");
        stream->errorFlag = true;
        delete [] packedbuf;
        return 0;
      }
    } else {
      memcpy(buffer, packedbuf, packedSize);
    }
    delete [] packedbuf;
    return count;
  } else {
    delete [] packedbuf;
    return 0;
  }
}

size_t RlePacked_fwrite(void* buffer, size_t size, size_t count, RLE_PACKED_FILE* stream) {
  assert(stream != NULL);
  assert(stream->outputFile != NULL);
  assert(size > 0 && count > 0);

  int unpackedSize = (int)size*(int)count;
  assert(unpackedSize > 0);

  unsigned char* packedbuf = new unsigned char[unpackedSize+16];

  scpak_set_bits(16);
  scpak_set_16bit_params(0xA0EA, 0xA0EB, 0xA0EC);
  int packedSize = scpak_pack(unpackedSize, (unsigned char*)buffer, packedbuf);
  
  char isPacked = 1;
  if(packedSize == 0) {
    isPacked = 0;
    packedSize = unpackedSize;
    memcpy(packedbuf, buffer, packedSize);
  }

  int tmpgot = 0;

  tmpgot = fwrite(&RLE_PACKED_CHUNK_MAGIC, sizeof(int), 1, stream->outputFile);
  if(tmpgot != 1) {
    assert(!"RlePacked_fwrte - write failed (chunk id).");
    stream->errorFlag = true;
    delete [] packedbuf;
    return 0;
  }

  tmpgot = fwrite(&isPacked, sizeof(char), 1, stream->outputFile);
  if(tmpgot != 1) {
    assert(!"RlePacked_fwrite - write failed (chunk packed flag).");
    stream->errorFlag = true;
    delete [] packedbuf;
    return 0;
  }

  tmpgot = fwrite(&packedSize, sizeof(int), 1, stream->outputFile);
  if(tmpgot != 1) {
    assert(!"RlePacked_fwrite - write failed (chunk packed size).");
    stream->errorFlag = true;
    delete [] packedbuf;
    return 0;
  }

  tmpgot = fwrite(&unpackedSize, sizeof(int), 1, stream->outputFile);
  if(tmpgot != 1) {
    assert(!"RlePacked_fwrite - write failed (chunk unpacked size).");
    stream->errorFlag = true;
    delete [] packedbuf;
    return 0;
  }

  int got = fwrite(packedbuf, packedSize, 1, stream->outputFile);
  if(got != 1) {
    assert(!"RlePacked_fwrite - write failed (packed data).");
    stream->errorFlag = true;
    delete [] packedbuf;
    return 0;
  }

  delete [] packedbuf;

  return count;
}

long RlePacked_fsize(RLE_PACKED_FILE* stream) {
  assert(stream != NULL);
  assert(stream->inputFile != NULL || stream->outputFile != NULL);

  if(stream->inputFile != NULL) {
#ifdef USE_SC_INPUT_STREAM_WRAPPER
  return sc_fsize(stream->inputFile);
#else
  fpos_t pos;
  if(fgetpos(stream->inputFile, &pos) != 0) {
    assert(!"fgetpos failed.");
    stream->errorFlag = true;
    return -1;
  }
  fseek(stream->inputFile, 0, SEEK_END);
  long tmp = ftell(stream->inputFile);

  if(fsetpos(stream->inputFile, &pos) != 0) {
    assert(!"fsetpos failed.");
    stream->errorFlag = true;
    return -1;
  }
  return tmp;
#endif
  } else {
    return 0;
  }
}

int RlePacked_fclose(RLE_PACKED_FILE* stream) {
  assert(stream != NULL);
  assert(stream->inputFile != NULL || stream->outputFile != NULL);

  int result = 0;
  if(stream->inputFile != NULL) {
#ifdef USE_SC_INPUT_STREAM_WRAPPER
    result = sc_fclose(stream->inputFile);
#else
    result = fclose(stream->inputFile);
#endif
    stream->inputFile = NULL;
  }
  if(stream->outputFile != NULL) {
    fclose(stream->outputFile);
    stream->outputFile = NULL;
  }
  return result;
}

bool RlePackedWasError(RLE_PACKED_FILE* stream) {
  assert(stream != NULL);
  return stream->errorFlag;
}

