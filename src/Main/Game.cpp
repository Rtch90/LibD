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

#include "../Level/Level.h"
#include "Game.h"
#include "TitleScreen.h"

Game::Game(void) {
  _level  = new Level();
  _player = new Player(_level);
  _NPC    = new NPC(_level);

  _NPC->SetXY(30.0f, 30.0f);

  _testFont = new Font();

  _titleScreen = new TitleScreen();
  _inTitleScreen = true;

  _running = true;
}

Game::~Game(void) {
}

bool Game::Init(void) {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.1f);

  // Return success.
  return true;
}

void Game::Prepare(float dt) {

}

void Game::Render(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if(_inTitleScreen) {
    RenderTitle();
  } else {
    RenderGame();
  }
}

void Game::Shutdown(void) {
  Debug::logger->message("\n ----- Cleaning Engine -----");
  delete _testFont;
  delete _NPC;
  delete _player;
  delete _level;
  delete _titleScreen;
}

void Game::ProcessEvents(float dt) {
  if(_inTitleScreen) {
    UpdateTitle(dt);
  } else {
    UpdateGame(dt);
  }
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
  _player->Update(dt);
  _NPC->Update(dt);
  _level->Update(dt);
}

void Game::RenderTitle(void) {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_ALPHA_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  _titleScreen->Render();
}

void Game::RenderGame(void) {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  float windowCenterX = ((float)windowWidth / 2.0f) - ((float)_player->GetMaxWidth() / 2.0f);
  float windowCenterY = ((float)windowHeight / 2.0f) - ((float)_player->GetMaxHeight() / 2.0f);

  float xOffset = _player->GetX() - windowCenterX;
  float yOffset = _player->GetY() - windowCenterY;

  float maxXOffset = (_level->GetWidth() * _level->GetTileWidth()) - (float)windowWidth;
  float maxYOffset = (_level->GetHeight() * _level->GetTileHeight()) - (float)windowHeight;

  if(xOffset < 0.0f) xOffset = 0.0f;
  if(yOffset < 0.0f) yOffset = 0.0f;
  if(xOffset > maxXOffset) xOffset = maxXOffset;
  if(yOffset > maxYOffset) yOffset = maxYOffset;

  glTranslatef((int)-xOffset, (int)-yOffset, 0.0f);

  // Render our shit..
  _level->Draw(xOffset, yOffset);
  _player->Render();
  _NPC->Render();
  _testFont->RenderText(
    _player->GetX() - 5,
    _player->GetY() - _testFont->GetLineSkip() - 2,
    "Miss D");
}

void Game::NewGame(void) {
  _level->Load("../Data/Map/Ugly.tmx");
  _level->PlayBGM();

  _player->LoadSprites("Player");
  _NPC->LoadSprites("Player");

  _testFont->Load("../Data/Font/Fairydust.ttf", 16);
  _testFont->SetColor(0.0f, 1.0f, 1.0f, 1.0f);

  _inTitleScreen = false;
}

void Game::LoadGame(void) {
}

void Game::Quit(void) {
  SetRunning(false);
}
