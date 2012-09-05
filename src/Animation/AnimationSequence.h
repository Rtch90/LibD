#pragma once
#include <string>
#include <assert.h>

#include "../System/Filesystem/FileReader.h"
#include "../System/String.h"

#define MAX_FRAMES    16
#define BLANK         10
#define ENDOFLINE     59
#define SPACE         32

using saracraft::util::String;
using saracraft::filesystem::FileReader;

struct Animation {
  String _animationID;
  int frameBegin;
  int frameEnd;
  int _loopTo;
};

class AnimationSequence {
public:
  AnimationSequence(void);
  AnimationSequence(const char* filename);
  ~AnimationSequence(void);

  void ReadFile(void);
  Animation* GetAnimation(int index);
  Animation* GetAnimation(const String& filename);

private:
  String Scan(char* source, int& counter);

  const char* _sequenceID;
  int _numberOfFrames;
  FileReader _file;
  Animation* _animations[MAX_FRAMES];
};
