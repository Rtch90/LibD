#pragma once

class FileTimestampChecker {
public:
  // Check if the given file with given name has been modified after another
  // file of given name.
  // Return true if file *IS* newer.

  // This is not needed....
  //static bool IsFileNewerThanFile(const char* file, const char* secondfile);
  static bool IsFileNewerOrSameThanFile(const char* file, const char* secondfile);
  static bool IsFileNewerOrAlmostSameThanFile(const char* file, const char* secondfile);
  static bool IsFileUpToDateComparedTo(const char* file, const char* secondfile);

  static int GetFileTimestamp(const char *file);
};

