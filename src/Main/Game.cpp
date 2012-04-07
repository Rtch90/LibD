#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include "Game.h"

Game::Game(void) {
	_rotationAngle = 0.0f;
}

Game::~Game(void) {

}

bool Game::Init(void) {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
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
	glLoadIdentity();
	
	glRotatef(_rotationAngle, 0, 0, 1);
	
	glBegin(GL_TRIANGLES);
	  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, -0.5f, -4.0f);
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, -0.5f, -4.0f);
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		glVertex3f(0.0f, 0.5f, -4.0f);
	glEnd();
}

void Game::Shutdown(void) {
	
}

void Game::OnResize(int width, int height) {
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45.0f, float(width) / float(height), 1.0f, 100.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
