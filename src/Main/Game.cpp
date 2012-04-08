#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include "Game.h"
#include "../Sprite/Sprite.h"
#include "../Texture/Texture.h"

Game::Game(void) {
	_rotationAngle = 0.0f;
}

Game::~Game(void) {

}

bool Game::Init(void) {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
  Texture* testTexture = new Texture();
  testTexture->Load("../Data/Img/test.png");
  
  _testSprite = new Sprite();
  _testSprite->SetTexture(testTexture);
  _testSprite->SetHandle(Vec2(800/2, 600/2));
  _testSprite->SetScale(Vec2(1.0f, 1.0f));

	// Return success.
	return true;
}

void Game::Prepare(float dt) {
	const float SPEED = 15.0f;
	_rotationAngle += SPEED*dt;
	if(_rotationAngle > 360.0f) {
    _rotationAngle -= 360.0f;
	}
}

void Game::Render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 800.0, 600.0, 0.0, 0.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
	
  _testSprite->SetRotation(_rotationAngle);
	_testSprite->Draw();
}

void Game::Shutdown(void) {
	delete _testSprite->GetTexture();
  delete _testSprite;
}

void Game::OnResize(int width, int height) {
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45.0f, float(width) / float(height), 1.0f, 100.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
