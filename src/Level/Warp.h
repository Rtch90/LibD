#pragma once

#include "../System/String.h"
#include "../Math/Vec2.h"

class Warp {
public:
  Warp(void);

  int   GetX(void) const { return x; }
  int   GetY(void) const { return y; }
  int   GetWidth(void) const { return w; }
  int   GetHeight(void) const { return h; }
  void  SetX(int x) { this->x = x; }
  void  SetY(int y) { this->y = y; }
  void  SetXY(int x, int y) { SetX(x); SetY(y); }
  void  SetWidth(int w)   { this->w =  w; }
  void  SetHeight(int h)  { this->h = h;}
  void  SetWidthHeight(int w, int h) { SetWidth(w); SetHeight(h); }

  const String& GetTargetMap() const { return _targetMap;}
  void SetTargetMap(const String& targetMap) { _targetMap = targetMap; }

  int   GetTargetX(void) const { return _targetX; }
  int   GetTargetY(void) const { return _targetY; }
  void  SetTargetX(int targetX) { _targetX = targetX; }
  void  SetTargetY(int targetY) { _targetY = targetY; }
  void  SetTargetXY(int targetX, int targetY) { SetTargetX(targetX); SetTargetY(targetY); }

private:
  int x;
  int y;
  int w;
  int h;

  String  _targetMap;
  int     _targetX;
  int     _targetY;
};
