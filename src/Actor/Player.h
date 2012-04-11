#pragma once
#include "../Sprite/Sprite.h"
#include "../Global/Globals.h"
#include "../System/Debug.h"
#include "../IO/Input.h"

class Sprite;

// We will derive from an Actor class at some point.
class Player {
public:
  Player(void);
  ~Player(void);

  void Prepare(void);
  void Render(void);
  void ProcessEvents(void);

private:
  float x;
  float y;
  float PLAYER_SPEED;
  Sprite*   _player;
  float     _rotationAngle;
};
