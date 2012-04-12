#pragma once
#include "../Math/Vec2.h"
#include "AABB.h"

// TileCollision stores collision data loaded from an
// an external collision map file. It stores the AABB,
// tile ID and chosen color.

class TileCollision {
public:
  TileCollision(void)  {}
  ~TileCollision(void) {}

  void SetAABB(AABB* aabb)                { _AABB.SetMax(aabb->GetMax()); _AABB.SetMin(aabb->GetMin()); }
  void SetAABB(Vec2 &min, Vec2 &max)      { _AABB.SetMax(max); _AABB.SetMin(min); }
  AABB* GetAABB(void)                     { return &_AABB; }
  void SetTileNumber(int tileID)          { _tileID = tileID; }
  int SetColor(int color)                 { _color = color; }
  int GetColor(void)                      { return _color; }

private:
  int _tileID;
  int _color;
  AABB _AABB;
};
