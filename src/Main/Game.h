#pragma once
#include "../IO/Input.h"
#include "../Actor/Player.h"
#include "../Font/Font.h"
#include "../BattleSys/Slot.h"

class Sprite;
class Level;
class Button;
class TitleScreen;
class Menu;

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

  void Warp(const String& mapName, int x, int y);

  bool IsRunning()              { return _running; }
  void SetRunning(bool running) { _running = running; }

private:
  // Battle system crap.
  Slot* _slot;

  void UpdateTitle(float dt);
  void UpdateGame(float dt);
  void RenderTitle(void);
  void RenderGame(void);
  void RenderHUD(void);

  void NewGame(void);
  void LoadGame(void);
  void Quit(void);

  Font*   _testFont;
  Player* _player;
  Level*  _level;

  TitleScreen* _titleScreen;
  bool         _inTitleScreen;

  Menu* _inGameMenu;
  bool _inGameMenuShown;

  bool _running;
};
