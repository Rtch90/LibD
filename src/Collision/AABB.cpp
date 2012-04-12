#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include "AABB.h"
#include "../Texture/Texture.h"

AABB::AABB(void) {
  _surface = NULL;
  _texture = NULL;
  // _max represents the top left.
  _max = Vec2(0,0);
  // _min is the bottom left.
  _min = Vec2(0,0);
}

AABB::AABB(Vec2 &min, Vec2 &max) {
  _max = max;
  _min = min;
  // static represent the initial position of the max and min
  // so we can position the AABB and retain it's initial values.
  _staticMin = min;
  _staticMax = max;
}

AABB::~AABB(void) {
  if(_surface) {
    SDL_FreeSurface(_surface);
    _surface = NULL;
  }
  if(_texture) {
    textureManager.Destroy(_texture);
    _texture = NULL;
  }
}

void AABB::SetRelativePosition(float x, float y) {
  _max.x += x;
  _min.x += x;
  _max.y += y;
  _min.y += y;
}

void AABB::SetPositionOffset(float x, float y) {
  // Allow the AABB to be repositioned.
  _max.x = _staticMax.x + x;
  _min.x = _staticMin.x + x;
  _max.y = _staticMax.y + y;
  _min.y = _staticMin.y + y;
}

bool AABB::InCollision(AABB* otherAABB) {
  if(_max.x < otherAABB->GetMin().x || _min.x > otherAABB->GetMax().x) {
    return false;
  }
  if(_max.y < otherAABB->GetMin().y || _min.y > otherAABB->GetMax().y) {
    return false;
  }
  return true;
}

void AABB::CreateAABBFromSprite(const char* filename) {
  std::string tempName(filename);
  tempName += ".png";

  _surface = IMG_Load(filename);
  if(!_surface) {
    return;
  }

  _texture = textureManager.Load(filename);
  if(_texture) {
    SDL_FreeSurface(_surface);
    return;
  }

  // I have no methods here, hopefully KonoM will have it
  // implemented real soon...
  //float spriteWidth  = _surface->w;
  //float spriteHeight = _surface->h;

  // Find the min, look through until we find a first instance of a white color.
  bool found = false;
  int color = 0;

  Uint32* pixels = (Uint32*)_surface->pixels;

  for(int width = 0; width < _surface->w; width++) {
    for(int height = 0; height < _surface->h; height++) {
      // FUCKING PAIN IN THE ASS MOTHERFUCKER!!!!
      Uint32 offset = height * screen->pitch + width;
      if(((Uint32)pixels[offset]) != 0 && !found) {
       _min = Vec2((float)width, (float)height);
       found = true;
       color = ((Uint32)pixels[offset]);
       // Break out of these god forsaken loops.
       width  = _surface->w;
       height = _surface->h;
      }
    }
  }

  // Let's try to find the max.x now..
  _max.x = (float)_surface->w;
  found = false;
  for(int width = (int)_min.x; width < _surface->w; width++) {
    Uint32 offset = (Uint32)_min.y * screen->pitch + width;
    if(((Uint32)pixels[offset] != color && !found)) {
      found = true;
      _max.x = (float)width;
    }
  }

  // Now for the max.y
  _max.y = (float)_surface->h;
  found = false;
  for(int height = (int)_min.y; height < _surface->w; height++) {
    Uint32 offset = height * screen->pitch + (Uint32)_min.x;
    if(((Uint32)pixels[offset]) != color && !found) {
      found = true;
      _max.y = (float)height;
      break;
    }
  }
  _staticMax = _max;
  _staticMin = _min;

  SDL_FreeSurface(_surface);
  _surface = NULL;
}

void AABB::Render(void) {
  glEnable(GL_TEXTURE_2D);
  BindTexture(_texture->GetTexID());

  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(_min.x, _min.y);
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(_max.x, _min.y);
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(_max.x, _max.y);
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(_min.x, _max.y);
  glEnd();
}
