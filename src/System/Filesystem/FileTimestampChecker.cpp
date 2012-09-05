#include <assert.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "InputStreamWrapper.h"
#include "FileTimestampChecker.h"

namespace saracraft {
namespace filesystem {

/*bool FileTimestampChecker::IsFileNewerThanFile(const char* file, const char* secondFile) {
  assert
}*/

bool FileTimestampChecker::IsFileNewerOrSameThanFile(const char* file, const char* secondfile) {
  if(GetFileTimestamp(file) >= GetFileTimestamp(secondfile))
    return true;
  else
    return false;
}

bool FileTimestampChecker::IsFileNewerOrAlmostSameThanFile(const char* file, const char* secondfile) {
  // WARNING: Max 60 seconds older file is also accepted as newer..
  if(GetFileTimestamp(file) + 60 >= GetFileTimestamp(secondfile))
    return true;
  else
    return false;
}

bool FileTimestampChecker::IsFileUpToDateComparedTo(const char* file, const char* secondfile) {
  FILE* f = fopen(file, "rb");
  if(f) {
    fclose(f);
    return FileTimestampChecker::IsFileNewerOrAlmostSameThanFile(file, secondfile);
  }

  filesystem::SC_FILE* fp = filesystem::sc_fopen(file, "rb");
  if(fp) {
    filesystem::sc_fclose(fp);
    return true;
  }
  return false;
}


#ifndef _MSC_VER
#define _stat stat

#ifndef _WIN32_
#define _fileno fileno
#endif

#define _fstat fstat
#endif


int FileTimestampChecker::GetFileTimestamp(const char* file) {
  struct _stat buf;
  FILE* f;
  int fh, result;
  int ret;

  f = fopen(file, "rb");
  if(f == NULL) return -1;

  fh = _fileno(f);

  result = _fstat(fh, &buf);

  if(result != 0) {
    return -1;
  } else {
    ret = int(buf.st_mtime);
  }
  fclose(f);
  return ret;
}

} // End of namespace filesystem.
} // End of namespace saracraft.
