#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#endif

#define GLX_GLXEXT_LEGACY		//  Use our local glxext.h rather than the system one.

#ifdef _WIN32
#include <windows.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <GL/gl.h>
#include <time.h>
#include "Game.h"
#include "../IO/Input.h"
#include "../Global/Globals.h"
#include "../Global/Constants.h"
#include "../System/Debug.h"

void Destroy(void) {
  TTF_Quit();
  Mix_CloseAudio();
  DestroyInput();
  SDL_FreeSurface(screen);
  SDL_Quit();
}

static void ResizeWindow(Game& game, int w, int h) {
  SDL_SetVideoMode(w, h, 32, SDL_OPENGL | SDL_RESIZABLE);
  game.OnResize(w, h);
  SDL_GL_SwapBuffers();
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow) {
#else
int main(int argc, char** argv) {
#endif
  // Start by opening a debug log.
  Debug::openLog(true);
  Debug::logger->message("\n ----- Engine Loading -----");

  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    Debug::logger->message("Error: Could not load SDL");
    Destroy();
    return 1;
  } else {
    Debug::logger->message("SDL loaded..");
  }

  // Setup OpenGL.
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            5);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          5);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           5);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,        1);

  flags = SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE;

  screen = SDL_SetVideoMode(windowWidth, windowHeight, 32, flags);
  Debug::logger->message("Video mode set..");

  if(Mix_OpenAudio(44100, AUDIO_S16, 2, 4096)) {
    Debug::logger->message("Audio opened..");
  }

  if(TTF_Init()) {
	  Debug::logger->message("SDL_ttf initialized.\n");
  }

  info = SDL_GetVideoInfo();
  if(!info) {
    // This should never accur.
    Debug::logger->message("Video query failed!");
    Destroy();
    return 1;
  }

  SDL_WM_SetCaption("LibD", NULL);

  srand((unsigned int)time(NULL));

  Debug::logger->message("\n ----- Engine Initialization Complete -----");
  Debug::logger->message("\n ----- Logic -----");

  Game game;
  game.Init();
  CreateInput();

  Uint32 timeStart = SDL_GetTicks();
  float dt = 1.0f / 60.0f;

  bool isRunning = true;
  while(isRunning) {

    while(SDL_PollEvent(&event)) {
      if((event.type == SDL_QUIT) || KeyStillDown(SDLK_ESCAPE)) {
        isRunning = false;
        break;
      }
      if(event.type == SDL_VIDEORESIZE) {
        // Resize the window.
        ResizeWindow(game, event.resize.w, event.resize.h);
        break;
      }
    }

    UpdateInput();
    game.ProcessEvents(dt);
    game.Render();
    SDL_GL_SwapBuffers();

    Uint32 timeEnd = SDL_GetTicks();
	  dt = (float)(timeEnd - timeStart) / 1000.0f;
	  timeStart = timeEnd;
  }

  game.Shutdown();

  Destroy();

  Debug::closeLog();
  return 0;
}
