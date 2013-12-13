#ifdef _WIN32
#include <windows.h>
#endif

#include <algorithm>

#include <GL/gl.h>
#include <GL/glu.h>

#include "../Global/Globals.h"
#include "../Global/Constants.h"
#include "../System/Debug.h"
#include "../Sprite/Sprite.h"
#include "../Texture/Texture.h"
#include "../UI/Button.h"
#include "../UI/Menu.h"
#include "../Sound/Music.h"
#include "../Sound/SoundEffect.h"

#include "../Level/Level.h"
#include "Game.h"
#include "TitleScreen.h"

Game::Game(void) {
  _level  = new Level(this);
  _player = new Player(_level);

  //_NPC    = new NPC(_level);
  _slot   = new Slot();

  //_NPC->SetXY(30.0f, 30.0f);

  _testFont = new Font();

  _titleScreen = new TitleScreen();
  _inTitleScreen = true;

  _inGameMenu = NULL;
  _inGameMenuShown = false;

  _running = true;
}

Game::~Game(void) {
}

bool Game::Init(void) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Return success.
  return true;
}

void Game::Prepare(float dt) {
  _slot->PrepareSlot();
}

void Game::Render(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  if(_inTitleScreen) {
    RenderTitle();
  } else {
    RenderGame();
  }
}

void Game::Shutdown(void) {
  Debug::logger->message("\n ----- Cleaning Engine -----");
  delete _testFont;
  delete _player;
  delete _level;
  delete _slot;
  if(_inGameMenu) {
    delete _inGameMenu;
    _inGameMenu = NULL;
  }
}

void Game::ProcessEvents(float dt) {
  if(_inTitleScreen) {
    UpdateTitle(dt);
  } else {
    UpdateGame(dt);
  }
  _slot->ProcessEvents();
}

void Game::OnResize(int width, int height) {
  glViewport(0, 0, width, height);

  windowWidth = width;
  windowHeight = height;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, (GLdouble)windowWidth, (GLdouble)windowHeight, 0.0, 0.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Game::Warp(const String& mapName, int x, int y) {
  Level* newLevel = new Level(this);

  if(!newLevel->Load(mapName.GetPointer())) {
    delete newLevel;
    return;
  }

  delete _level;
  _level = newLevel;

  _player->SetXY(x, y);
  _player->SetLevel(_level);

  _level->PlayBGM();
}

void Game::UpdateTitle(float dt) {
  _titleScreen->Update(dt);

  if(!_titleScreen->IsAlive()) {
    switch(_titleScreen->GetResult()) {
    case TitleScreen::NEW_GAME:
      NewGame();
      break;

    case TitleScreen::LOAD_GAME:
      LoadGame();
      break;

    case TitleScreen::QUIT:
      Quit();
      break;
    }
  }
}

void Game::UpdateGame(float dt) {
  if(KeyDown(SDLK_F1)) {
    _inGameMenuShown = !_inGameMenuShown;
  }
  if(_inGameMenuShown) {
    _inGameMenu->SetXY(windowWidth / 2, windowHeight / 2);
    _inGameMenu->Update();

    switch(_inGameMenu->GetTriggeredButton()) {
    case 0:
      SoundEffect::SetVolume(-1, 0);
      Music::SetVolume(0);
      break;

    case 1:
      SoundEffect::SetVolume(-1, 100);
      Music::SetVolume(100);
      break;
    }
  } else {
    _player->Update(dt);
    _level->Update(dt);
  }
}

void Game::RenderTitle(void) {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  _titleScreen->Render();
}

void Game::RenderGame(void) {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  float xOffset;
  float yOffset;

  int levelWidthPixels = _level->GetWidth() * _level->GetTileWidth();
  int levelHeightPixels = _level->GetHeight() * _level->GetTileHeight();

  if(levelWidthPixels < windowWidth && levelHeightPixels < windowHeight) {
    xOffset = levelWidthPixels / 2 - windowWidth / 2;
    yOffset = levelHeightPixels / 2 - windowHeight / 2;
    glTranslatef((int)-xOffset, (int)-yOffset, 0.0f);
  }
  else
  {
    float windowCenterX = ((float)windowWidth / 2.0f) - ((float)_player->GetMaxWidth() / 2.0f);
    float windowCenterY = ((float)windowHeight / 2.0f) - ((float)_player->GetMaxHeight() / 2.0f);

    xOffset = _player->GetX() - windowCenterX;
    yOffset = _player->GetY() - windowCenterY;

    float maxXOffset = (float)(levelWidthPixels - windowWidth);
    float maxYOffset = (float)(levelHeightPixels - windowHeight);

    if(xOffset < 0.0f) xOffset = 0.0f;
    if(yOffset < 0.0f) yOffset = 0.0f;
    if(xOffset > maxXOffset) xOffset = maxXOffset;
    if(yOffset > maxYOffset) yOffset = maxYOffset;

    glTranslatef((int)-xOffset, (int)-yOffset, 0.0f);
  }

  // Render our shit..
  _level->DrawBackground(xOffset, yOffset, _player->GetY());
  _player->Render();
  _level->DrawForeground(xOffset, yOffset, _player->GetY());
  _testFont->SetColor(0.0f, 1.0f, 1.0f, 1.0f);
  _testFont->RenderText(
    _player->GetX() - 5,
    _player->GetY() - _testFont->GetLineSkip() - 20,
    "Miss D");
  _testFont->RenderText(
    _player->GetX() - 50,
    _player->GetY() - _testFont->GetLineSkip() - 2,
    "<Misteress of Magic>");

  RenderHUD();
}

void Game::RenderHUD(void) {
  glLoadIdentity();
  if(_inGameMenuShown) {
    _inGameMenu->Render();
  }
  _slot->Render(windowWidth/600, windowHeight/1.10f);
}

void Game::NewGame(void) {
  _level->Load("../Data/Map/Ugly.tmx");
  _level->PlayBGM();

  _player->LoadSprites("Player");

  _testFont->Load("../Data/Font/Fairydust.ttf", 16);
  _testFont->SetColor(0.0f, 1.0f, 1.0f, 1.0f);

  Button* muteSoundsButton = new Button();
  muteSoundsButton->SetFont(_testFont);
  muteSoundsButton->SetText("Mute Sounds");

  Button* unmuteSoundsButton = new Button();
  unmuteSoundsButton->SetFont(_testFont);
  unmuteSoundsButton->SetText("Unmute Sounds");

  _inGameMenu = new Menu();
  _inGameMenu->AddButton(muteSoundsButton);
  _inGameMenu->AddButton(unmuteSoundsButton);
  _inGameMenu->AlignButtons(Menu::ALIGN_VERTICALLY);

  _inTitleScreen = false;
  delete _titleScreen;
}

void Game::LoadGame(void) {
}

void Game::Quit(void) {
  SetRunning(false);
  delete _titleScreen;
}
