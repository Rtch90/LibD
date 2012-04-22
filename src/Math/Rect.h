#pragma once

#include "Vec2.h"

struct Rect
{
  float x;
  float y;
  float w;
  float h;

  Rect(float x, float y, float w, float h)
  {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
  }

  bool CollidesWith(const Rect& other) const {
    if(x > (other.x + other.w))
      return false;

    if(y > (other.y + other.h))
      return false;

    if((x + w) < other.x)
      return false;

    if((y + h) < other.y)
      return false;

    return true;
  }
};
