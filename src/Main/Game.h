#pragma once
#include "../IO/Input.h"
#include "../Actor/NPC.h"
#include "../Actor/Player.h"
#include "../Font/Font.h"

class Sprite;
class Level;
class Button;
class TitleScreen;

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

  bool IsRunning()              { return _running; }
  void SetRunning(bool running) { _running = running; }

private:
  void UpdateTitle(float dt);
  void UpdateGame(float dt);
  void RenderTitle(void);
  void RenderGame(void);

  Font*   _testFont;
  Player* _player;
  NPC*    _NPC;
  Level*  _level;

  TitleScreen* _titleScreen;
  bool         _inTitleScreen;

  bool _running;
};
