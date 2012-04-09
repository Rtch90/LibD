#include <GL/gl.h>
#include "Globals.h"

static void ResizeWindow(int w, int h) {
  SDL_SetVideoMode(w, h, 32, SDL_OPENGL | SDL_RESIZABLE);
  glViewport(0,0,w,h);
  glLoadIdentity();
  glOrtho(0.0, 800.0, 600.0, 0.0, 0.0, 1.0);
  SDL_GL_SwapBuffers();
}

SDL_Surface* screen   = NULL;
SDL_Event event;

