#include "AABB.h"


typedef unsigned int DWORD;

AABB::AABB(void) {
  _sprite = 0;
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
  if(_sprite)
  delete _sprite;
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
  _sprite = new Sprite();
  _sprite->LoadSprite(tempName);

  // I have no methods here, hopefully KonoM will have it
  // implemented real soon...
  //float spriteWidth  = _sprite->GetWidth();
  //float spriteHeight = _sprite->GetHeight();

  // Find the min, look through until we find a first instance of a white color.
  bool found = false;
  int color = 0;
  DWORD* pixels = (DWORD*)screen->pixels;

  for(int width = 0; width < _sprite->GetWidth(); width++) {
    for(int height = 0; height < _sprite->GetHeight(); height++) {
      // FUCKING PAIN IN THE ASS MOTHERFUCKER!!!!
      DWORD offset = height * screen->pitch + width;
      if(((DWORD)pixels[offset]) != 0 && !found) {
       _min = Vec2((float)width, (float)height);
       found = true;
       color = ((DWORD)pixels[offset]);
       // Break out of these god forsaken loops.
       width  = _sprite->GetWidth();
       height = _sprite->GetHeight();
      }
    }
  }

  // Let's try to find the max.x now..
  _max.x = (float)_sprite->GetWidth();
  found = false;
  for(int width = (int)_min.x; width < _sprite->GetWidth(); width++) {
    DWORD offset = _min.y * screen->pitch + width;
    if(((DWORD)pixels[offset] != color && !found)) {
      found = true;
      _max.x = (float)width;
    }
  }

  // Now for the max.y
  _max.y = (float)_sprite->GetHeight();
  found = false;
  for(int height = (int)_min.y; height < _sprite->GetWidth(); height++) {
    DWORD offset = (DWORD)(height * screen->pitch + _min.x);
    if(((DWORD)pixels[offset]) != color && !found) {
      found = true;
      _max.y = (float)height;
      break;
    }
  }
  _staticMax = _max;
  _staticMin = _min;
  delete _sprite;
  _sprite = 0;
}
