#include <stdlib.h>

#include "../System/Debug.h"
#include "AnimationSequence.h"

/*
 * Load and read a sequence file for an animating sprite then
 * stores the result so you can call the sprites animation at any time.
 * This allows us to define animation cycles externally.
 *
 * The file stucture is:
 * [# of animation cycles] [name] [start frame] [end frame]
 * A typical cycle file should look something like:
 *
 * 3;                       -- Three cycles.
 * WalkLeft 1 20            -- [Name] [Frame 1 to 20]
 * WalkRight 42 62          -- [Name] [Frame 42 to 62]
 */

AnimationSequence::AnimationSequence(void) {

}

AnimationSequence::AnimationSequence(const char* filename) {
  _numberOfFrames   = 0;
  _sequenceID       = filename;
  ReadFile();
}

AnimationSequence::~AnimationSequence(void) {

}

void AnimationSequence::ReadFile(void) {
  // Open and read a file, storing it into a char buffer which
  // is then sorted character by character arranging the data into
  // usable animations using the scan method, each result is stored into an
  // animation array.
  if(_file.Exists(_sequenceID)) {
    String name;
    String loop;
    char* temp;
    _file.OpenFile(_sequenceID, "r");
    _file.ReadBuffer(temp);
    _file.CloseFile();

    int counter = 0;

    _numberOfFrames = atoi(Scan(temp, counter));

    for(int index = 0; index < _numberOfFrames; index++) {
      name              = "";
      int startFrame    = 0;
      int endFrame      = 0;
      loop              = "";

      name          = Scan(temp, counter);
      startFrame    = atoi(Scan(temp, counter));
      endFrame      = atoi(Scan(temp, counter));

      if(temp[counter - 1] == SPACE) {
        loop = Scan(temp, counter);
      }

      _animations[index] = new Animation();
      _animations[index]->_animationID  = name;
      _animations[index]->frameBegin    = startFrame;
      _animations[index]->frameEnd      = endFrame;
      _animations[index]->_loopTo       = loop;
    }
  } else {
    Debug::logger->message("%s does not exist.", _sequenceID);
    assert(false);
  }
}

const char* AnimationSequence::Scan(char* source, int &counter) {
  String temp;
  temp = "";
  bool terminate = false;
  while(!terminate) {
    if(source[counter] != SPACE && source[counter] != ENDOFLINE) {
      if(source[counter] == BLANK) {
        counter++;
      } else {
        temp.Concatenate(source[counter]);
        counter++;
      }
    } else {
      terminate = true;
      counter++;
    }
  }
  return temp;
}

AnimationSequence::Animation* AnimationSequence::GetAnimation(const char* filename) {
  for(int i = 0; i < _numberOfFrames; i++) {
    if(strcmp(filename, _animations[i]->_animationID) == 0) {
      return _animations[i];
    }
  }
  return 0;
}

AnimationSequence::Animation* AnimationSequence::GetAnimation(int index) {
  if(index < _numberOfFrames) {
    return _animations[index];
  }
  return 0;
}
