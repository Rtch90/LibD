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
  // Facing enum controls which spritr to render.
  enum Facing {
    LEFT,
    RIGHT,
    NONE
  };

  // Control the current state the character is in.
  enum Status {
    STANDING = 0,
    WALKING,
    HURT,
    DEAD,
  };

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

  Facing _preventMovement;

  SoundEffect* _stepSFX[4];
  int _lastStepSFXPlayed;
};
