#include <stdlib.h>
#include <assert.h>

#include "../Debug.h"
#include "FileReader.h"

namespace saracraft {
namespace filesystem {

FileReader::FileReader(void) {

}

FileReader::~FileReader(void) {

}

bool FileReader::Exists(const char* filename) {
  // Check to see if _filename is existent in memory,
  _file = fopen(filename, "rb");

  if(_file) {
    // Close the file we have.
    fclose(_file);
    _file = NULL;
    return true;
  } else {
    // Create the file.
    _file = fopen(filename, "w");
    fclose(_file);
    _file = NULL;
    return true;
  }
  return false;
}

void FileReader::OpenFile(const char* filename, const char* accessType) {
  // Open _filename into memory, passing in the access type.
  _filename = filename;
  _accessType = accessType;
  _file = fopen(filename, accessType);
}

void FileReader::CloseFile(void) {
  if(_file) {
    fclose(_file);
    _file = NULL;
  }
}

void FileReader::Write(const char* buffer) {
  if(_file) {
    fprintf(_file, "%s", buffer);
  } else {
    // _filename does not exist or we have the wrong accessType.
    assert(false);
  }
}

void FileReader::Write(const int buffer) {
  if((_file) && (_accessType == "w")) {
    fprintf(_file, "%i", buffer);
  } else {
    // _filename does not exist or we have the wrong accessType.
    assert(false);
  }
}

void FileReader::Read(const int &value) {
  if((_file) && (_accessType == "rb")) {
    fscanf(_file, "%i", &value);
  } else {
    // _filename does not exist or we have the wrong accessType.
    assert(false);
  }
}

void FileReader::WriteBuffer(const char* buffer) {
  if((_file) && (_accessType == "w")) {
    fwrite(buffer, 1, strlen(buffer), _file);
  }
}

void FileReader::WriteBuffer(const char* buffer, int count) {
  if((_file) && (_accessType == "w")) {
    fwrite(buffer, 1, count, _file);
  }
}

void FileReader::ReadBuffer(char* &buffer) {
  if((_file) && (_accessType == "rb")) {
    size_t size = 0;
    fseek(_file, 0, SEEK_END);
    size = ftell(_file);
    rewind(_file);

    buffer = (char*)malloc(sizeof(char)* size + sizeof(char));
    buffer[size] = 0;

    if(buffer != NULL) {
      fread(buffer, 1, size, _file);
    }
  }
}

void FileReader::ReadBuffer(const char* buffer, int count) {
  if((_file) && (_accessType == "r")) {
    buffer = (char*)malloc(sizeof(char)*count);

    if(buffer != NULL) {
      fread((char*) buffer, 1, count, _file);
    }
  }
}

} // Namespace filesystem.
} // Namespace saracraft.
