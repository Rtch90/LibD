#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#endif

#define GLX_GLXEXT_LEGACY		//  Use our local glxext.h rather than the system one.

#ifdef _WIN32
#include <windows.h>
#endif

#include <SDL/SDL.h>
#include <GL/gl.h>
#include "Game.h"
#include "../Global/Globals.h"
#include "../Global/Constants.h"
#include "../System/Debug.h"

void Destroy(void) {
  SDL_FreeSurface(screen);
  SDL_Quit();
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow) {
#else
int main(int argc, char** argv) {
#endif
  // Start by opening a debug log.
  Debug::openLog(true);
  Debug::logger->message("\n ----- Engine Loading -----");
  // Our game code.
  Game game;

  // Setup OpenGL.
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            5);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          5);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           5);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,        1);

  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    Debug::logger->message("Error: Could not load SDL");
  } else {
    Destroy();
    Debug::logger->message("SDL loaded..");
  }

  info = SDL_GetVideoInfo();
  if(!info) {
    // This should never accur.
    Debug::logger->message("Video query failed!");
    Destroy();
  }

  flags = SDL_OPENGL | SDL_HWSURFACE;

  screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, flags);
  Debug::logger->message("Video mode set..");

  SDL_WM_SetCaption("LibD", NULL);

  srand((unsigned int)time(NULL));

  Debug::logger->message("\n ----- Engine Initialization Complete -----");
  Debug::logger->message("\n ----- Logic -----");

  bool isRunning = true;
  while(isRunning) {
    break;
  }

  Destroy();

  Debug::closeLog();
  return 0;
}
