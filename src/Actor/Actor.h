#pragma once
#include <SDL/SDL.h>

#include "../Sprite/Sprite.h"
#include "../Math/Vec2.h"

class SoundEffect;

class Actor {
public:
  Actor(void);
  ~Actor(void);

  void LoadSprite(const char* filename);

  virtual void Update(float dt);
  virtual void Render(void);

  float GetX(void)                      { return x; }
  float GetY(void)                      { return y; }
  float GetWidth(void)                  { return w; }
  float GetHeight(void)                 { return h; }

  void SetXY(float xArg, float yArg)    { x = xArg; y = yArg; }

  int GetDirection(void)                { return _direction; }
  void SetDirection(int direction)      { _direction = direction; }

protected:
  virtual void Move(float dt) = 0;

  float _velocity;
  int _direction;

  static const int ANIM_LEFT_FOOT    = 0;
  static const int ANIM_NO_FOOT      = 1;
  static const int ANIM_RIGHT_FOOT   = 2;
  static const int ANIM_ATTACK       = 3;

  Sprite* _actor;

  float x;
  float y;

private:
  float w;
  float h;

  Vec2 _spriteVector[4][4];

  SoundEffect* _stepSFX[4];
  int _lastStepSFXPlayed;
};
