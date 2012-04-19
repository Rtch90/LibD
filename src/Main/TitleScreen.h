#pragma once

#include "../UI/Menu.h"

class Font;

class TitleScreen {
public:
  enum {
    NEW_GAME,
    LOAD_GAME,
    QUIT
  };

  TitleScreen(void);
  ~TitleScreen(void);
  
  void Update(float dt);
  void Render(void);

  bool  IsAlive(void)   { return _alive; }
  int   GetResult(void) { return _result; }

private:
  bool  _alive;
  int   _result;

  Font* _font;
  Menu  _menu;
};