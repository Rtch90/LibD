#pragma once
#include "../Sprite/Sprite.h"
#include "../Global/Globals.h"
#include "../System/Debug.h"
#include "../IO/Input.h"

class Sprite;
class SoundEffect;

// We will derive from an Actor class at some point.
class Player {
public:
  Player(void);
  ~Player(void);

  void Update(void);
  void Render(void);

  // --- Collision stuff.

  void ProcessEvents(void);

  int GetX(void) { return x; }
  int GetY(void) { return y; }
  int GetWidth(void);
  int GetHeight(void);

private:
  float x;
  float y;
  float PLAYER_SPEED;
  Sprite*   _player;
  float     _rotationAngle;

  SoundEffect* _stepSFX[4];
  int _lastStepSFXPlayed;
};
