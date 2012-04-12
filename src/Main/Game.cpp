#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include "../Global/Globals.h"
#include "../System/Debug.h"
#include "../Sprite/Sprite.h"
#include "../Texture/Texture.h"
#include "../Level/Level.h"
#include "Game.h"

Game::Game(void) {
  _player = new Player();
  _level = new Level();
  //_rotationAngle = 0.0f;
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
  _player->Prepare();

  // Return success.
  return true;
}

void Game::Prepare(float dt) {

}

void Game::Render(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 800.0, 600.0, 0.0, 0.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(-(_player->GetX() - 256), -(_player->GetY() - 128), 0.0f);

  // Render our shit..
  _level->Draw(_player->GetX() - 256, _player->GetY() - 128);
  _player->Render();

  glPopMatrix();
}

void Game::Shutdown(void) {
  Debug::logger->message("\n ----- Cleaning Engine -----");
  delete _player;
  delete _level;
}

void Game::ProcessEvents(void) {
  _player->ProcessEvents();
}

void Game::OnResize(int width, int height) {
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 800.0, 0.0, 600.0, 0.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
