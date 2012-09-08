#pragma once
#include <string>
#include <assert.h>

#include "../System/String.h"

#define MAX_FRAMES    16
#define BLANK         10
#define ENDOFLINE     59
#define SPACE         32

using saracraft::util::String;

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
  Animation* _animations[MAX_FRAMES];
};
