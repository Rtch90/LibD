#pragma once
#include <string>
#include <assert.h>

#include "../System/FileReader.h"
#include "../System/String.h"

#define MAX_FRAMES    16
#define BLANK         10
#define ENDOFLINE     59
#define SPACE         32

class AnimationSequence {
  struct Animation {
    const char* _animationID;
    int frameBegin;
    int frameEnd;
    String _loopTo;
  };

public:
  AnimationSequence(void);
  AnimationSequence(const char* filename);
  ~AnimationSequence(void);

  void ReadFile(void);
  Animation* GetAnimation(int index);
  Animation* GetAnimation(const char* filename);

private:
  const char* Scan(char* source, int& counter);

  const char* _sequenceID;
  int _numberOfFrames;
  FileReader _file;
  Animation* _animations[MAX_FRAMES];
};
