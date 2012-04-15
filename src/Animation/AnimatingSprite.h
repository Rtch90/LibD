#pragma once
#include "../Sprite/Sprite.h"
#include "AnimationSequence.h"
#define MAX_ANIM_FRAMES 128

class AnimatingSprite {
public:
  AnimatingSprite(void);
  ~AnimatingSprite(void);

  void SetCurrentAnimation(const char* filename);
  void SetCurrentAnimation(int index);
  const char* GetCurrentAnimation(void);

  int GetCurrentFrame(void) { return _currentFrame; }
  int GetTotalFrames(void)  { return _sequence->GetAnimation(_currentAnimation)->frameEnd; }

  void Update(float dt);

  void Render(void);
  void Render(float x, float y);

  void LoadAnimatingSprite(const char* id, const char* filename, const char* sequence, int frames, float animationSpeed);

  const char* GetID(void)   { return _id; }

private:
  Sprite* _sprites[MAX_ANIM_FRAMES];
  int _spriteCounter;
  AnimationSequence* _sequence;

  const char*   _id;
  float         _animationSpeed;
  float         _timer;
  int           _currentFrame;
  int           _numberOfFrames;

  const char*   _currentAnimation;
};
