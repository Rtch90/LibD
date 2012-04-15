#pragma once
#include <SDL/SDL.h>

#include "../Sprite/Sprite.h"
#include "../Animation/AnimatingSprite.h"
#include "../Math/Vec2.h"

class SoundEffect;

class Actor {
public:

  enum Facing {
    Front,
    BACK,
    LEFT,
    RIGHT,
    NONE
  };

  enum Status {
    WALKING,
    HURT
  };

  Actor(void);
  ~Actor(void);

  void LoadSprite(const char* filename);

  virtual void Update(float dt);
  virtual void Render(void);
  virtual void Render(float x, float y);

  float GetX(void)                      { return x; }
  float GetY(void)                      { return y; }
  float GetWidth(void)                  { return w; }
  float GetHeight(void)                 { return h; }

  void SetXY(float xArg, float yArg)    { x = xArg; y = yArg; }

  Facing GetDirection(void)                 { return _direction; }
  void SetDirection(Facing direction)      { _direction = direction; }

protected:
  virtual void Move(float dt) = 0;

  float _velocity;

  Sprite* _actor;
  AnimatingSprite* _actorLeft;
  AnimatingSprite* _actorRight;
  AnimatingSprite* _actorFront;
  AnimatingSprite* _actorBack;

  Facing _direction;
  Facing _preventMovement;

  float x;
  float y;

private:
  float w;
  float h;

  SoundEffect* _stepSFX[4];
  int _lastStepSFXPlayed;
};
