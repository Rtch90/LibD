#pragma once

#include <string>
#include "../Math/Vec2.h"

class Sprite;

class Tileset {
public:
  Tileset(int tileWidth, int tileHeight);
  ~Tileset();

  bool LoadImage(const std::string& filename);

  void DrawTile(int id, const Vec2& position);

private:
  int _tileWidth;
  int _tileHeight;
  Sprite* _sprite;
  int _widthInTiles;
};