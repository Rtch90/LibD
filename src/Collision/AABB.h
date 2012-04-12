#pragma once
#include <string>

#include "../Math/Vec2.h"
#include "../Global/Globals.h"

/*
 * The axis aligned bounding box contains
 * two vectors to represent the top left
 * and bottom right of a box. This can
 * then be used to calculate collision
 * against other bounding box's.
 */

struct SDL_Surface;

class AABB {
public:
  AABB(void);
  AABB(Vec2 &min, Vec2 &max);
  ~AABB(void);

  Vec2 GetMax(void)                     { return _max; }
  void SetMax(Vec2 &max)                { SetMax(max.x, max.y); }
  void SetMax(float maxX, float maxY)   { _max.x = maxX; _max.y = maxY; _staticMax = _max; }

  Vec2 GetMin(void)                     { return _min; }
  void SetMin(Vec2 &min)                { SetMin(min.x, min.y); }
  void SetMin(float minX, float minY)   { _min.x = minX; _min.y = minY; _staticMin = _min; }

  void SetRelativePosition(float x, float y);
  void SetPositionOffset(float x, float y);
  bool InCollision(AABB* otherAABB);
  void CreateAABBFromSprite(const char* filename);

private:
  Vec2 _min;
  Vec2 _max;

  Vec2 _staticMin;
  Vec2 _staticMax;

  SDL_Surface* _sprite;
};
