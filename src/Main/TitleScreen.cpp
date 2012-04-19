#include "TitleScreen.h"
#include "../Font/Font.h"
#include "../UI/Button.h"
#include "../Global/Globals.h"

TitleScreen::TitleScreen(void) {
  _alive = true;
  _result = TitleScreen::QUIT;

  _font = new Font();
  _font->Load("../Data/Font/fairydust.ttf", 24);

  Button* newGameButton = new Button();
  Button* loadGameButton = new Button();
  Button* quitButton = new Button();

  newGameButton->SetFont(_font);
  loadGameButton->SetFont(_font);
  quitButton->SetFont(_font);

  newGameButton->SetText("New Game");
  loadGameButton->SetText("Load Game");
  quitButton->SetText("Quit");

  _menu.AddButton(newGameButton);
  _menu.AddButton(loadGameButton);
  _menu.AddButton(quitButton);
  _menu.AlignButtons(Menu::ALIGN_VERTICALLY);
}

TitleScreen::~TitleScreen(void) {
  if(_font) {
    delete _font;
    _font = NULL;
  }
}

void TitleScreen::Update(float dt) {
  _menu.Update();
  switch(_menu.GetTriggeredButton()) {
  case 0:
    _alive = false;
    _result = TitleScreen::NEW_GAME;
    break;

  case 1:
    _alive = false;
    _result = TitleScreen::LOAD_GAME;
    break;

  case 2:
    _alive = false;
    _result = TitleScreen::QUIT;
    break;
  }
}

void TitleScreen::Render(void) {
  _menu.SetXY(32, windowHeight - 128);
  _menu.Render();
}
