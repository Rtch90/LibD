#pragma once

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
};
