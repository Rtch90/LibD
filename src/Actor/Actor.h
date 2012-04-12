#pragma once
#include <SDL/SDL.h>

#include "../Sprite/Sprite.h"
#include "../Math/Vec2.h"

class Actor {
public:
  enum State {
    WALKING,
    RUNNING,
    ATTACKING,
  };

  enum Facing {
    FRONT,
    BACK,
    LEFT,
    RIGHT
  };

  Actor(void);
  ~Actor(void);

  void LoadSprite(const char* filename, float w, float h);
  void Update(void);
  void Render(void);

  float GetX(void)                      { return x; }
  float GetY(void)                      { return y; }
  float GetWidth(void)                  { return w; }
  float GetHeight(void)                 { return h; }

  void SetXY(float xArg, float yArg)    { x = xArg; y = yArg; }

  int GetDirection(void)                { return _direction; }
  void SetDirection(int direction)      { _direction = direction; }

protected:
  const float VELOCITY;

  int _direction;

  static const int ANIM_LEFT_FOOT    = 0;
  static const int ANIM_NO_FOOT      = 1;
  static const int ANIM_RIGHT_FOOT   = 2;
  static const int ANIM_ATTACK       = 3;

private:
  float x;
  float y;
  float w;
  float h;

  Sprite* _actor;
  SDL_Surface* texture;
  Vec2 _spriteVector[4][4];

};
