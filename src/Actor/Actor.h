#pragma once
#include <SDL/SDL.h>

#include "../Sprite/Sprite.h"
#include "../Animation/AnimatingSprite.h"
#include "../Math/Vec2.h"

class SoundEffect;
class Level;

class Actor {
public:

  enum Facing {
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    NONE
  };

  enum Status {
    WALKING,
    HURT
  };

  Actor(const Level* level);
  ~Actor(void);

  void LoadSprites(const String& basename);

  virtual void Update(float dt);
  virtual void Render(void);
  virtual void Render(float x, float y);

  float GetX(void)                      { return x; }
  float GetY(void)                      { return y; }
  float GetWidth(void);
  float GetHeight(void);
  float GetMaxWidth(void);
  float GetMaxHeight(void);

  void SetXY(float xArg, float yArg)    { x = xArg; y = yArg; }

  Facing GetDirection(void)             { return _direction; }
  void SetDirection(Facing direction)   { _direction = direction; }

protected:
  virtual void Move(float dt) = 0;

  AnimatingSprite* GetAnimation(void);

  const Level* _level;

  float _velocity;

  AnimatingSprite* _actorLeft;
  AnimatingSprite* _actorRight;
  AnimatingSprite* _actorFront;
  AnimatingSprite* _actorBack;

  Facing _direction;
  Facing _preventMovement;

  float x;
  float y;

private:

  SoundEffect* _stepSFX[4];
  int _lastStepSFXPlayed;

  String _walkAnimationID;
};
