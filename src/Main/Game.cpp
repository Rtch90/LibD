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

Game::Game(void) {
  _player = new Player();
  _NPC    = new NPC();
  _level  = new Level();

  _player->LoadSprite("../Data/Img/Player.png");
  _NPC->LoadSprite("../Data/Img/Player.png");

  _NPC->SetXY(30.0f, 30.0f);

  _testFont = new Font();
  _testFont->Load("../Data/Font/Fairydust.ttf");
  _testFont->SetColor(0.0f, 1.0f, 1.0f, 1.0f);
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

  _level->Load("../Data/Map/Ugly.tmx");
  _level->PlayBGM();

  // Return success.
  return true;
}

void Game::Prepare(float dt) {

}

void Game::Render(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  float windowCenterX = ((float)windowWidth / 2.0f) - ((float)_player->GetWidth() / 2.0f);
  float windowCenterY = ((float)windowHeight / 2.0f) - ((float)_player->GetHeight() / 2.0f);

  float xOffset = _player->GetX() - windowCenterX;
  float yOffset = _player->GetY() - windowCenterY;

  float maxXOffset = (_level->GetWidth() * _level->GetTileWidth()) - (float)windowWidth;
  float maxYOffset = (_level->GetHeight() * _level->GetTileHeight()) - (float)windowHeight;

  if(xOffset < 0.0f) xOffset = 0.0f;
  if(yOffset < 0.0f) yOffset = 0.0f;
  if(xOffset > maxXOffset) xOffset = maxXOffset;
  if(yOffset > maxYOffset) yOffset = maxYOffset;

  glTranslatef(-xOffset, -yOffset, 0.0f);

  // Render our shit..
  _level->Draw(xOffset, yOffset);
  _player->Render();
  _NPC->Render();
  _testFont->DrawText(
    _player->GetX() - 5,
    _player->GetY() - _testFont->GetLineSkip() - 2,
    "Miss D");
}

void Game::Shutdown(void) {
  Debug::logger->message("\n ----- Cleaning Engine -----");
  delete _testFont;
  delete _NPC;
  delete _player;
  delete _level;
}

void Game::ProcessEvents(float dt) {
  _player->Update(dt);
  _NPC->Update(dt);
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
