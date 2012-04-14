#pragma once
#include <string.h>
#include <stdio.h>

class FileReader {
public:
  FileReader(void);
  ~FileReader(void);

  void OpenFile(const char* filename, const char* accessType);
  void CloseFile(void);
  void Write(const char* buffer);
  void Write(const int value);
  void Read(const char* buffer);
  void Read(const int &value);

  void WriteBuffer(const char* buffer);
  void WriteBuffer(const char* buffer, int count);
  void ReadBuffer(char* &buffer);
  void ReadBuffer(const char* buffer, int count);

  bool Exists(const char* filename);

private:
  FILE* _file;
  std::string _filename;
  std::string _accessType;
};
