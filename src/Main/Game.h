#pragma once
#include "../IO/Input.h"
#include "../Actor/NPC.h"
#include "../Actor/Player.h"
#include "../Font/Font.h"

class Sprite;
class Level;

class Game {
public:
  Game(void);
  ~Game(void);

  bool Init(void);
  void Prepare(float dt);
  void Render(void);
  void Shutdown(void);

  void ProcessEvents(float dt);

  void OnResize(int width, int height);

private:
  Font*   _testFont;
  Player* _player;
  NPC*    _NPC;
  Level*  _level;
};
