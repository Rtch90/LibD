#pragma once
#include "../IO/Input.h"
#include "../Actor/Player.h"

class Sprite;

class Game {
public:
  Game(void);
  ~Game(void);

  bool Init(void);
  void Prepare(float dt);
  void Render(void);
  void Shutdown(void);

  void ProcessEvents(void);

  void OnResize(int width, int height);

private:
  Player* _player;
};
