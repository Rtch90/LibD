#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#endif

#define GLX_GLXEXT_LEGACY		//  Use our local glxext.h rather than the system one.

#ifdef _WIN32
#include <windows.h>
#endif

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

  if(SDL_Init(SDL_INIT_VIDEO == -1)) {
    Debug::logger->message("Error: Could not load SDL");
  } else {
    Debug::logger->message("SDL loaded..");
  }

  screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_HWSURFACE);
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
