#pragma once

#include "MapTile.h"

class Layer {
public:
  Layer(int width, int height, int tileWidth, int tileHeight);
  ~Layer();

  void Update(float dt);
  void Draw(int xOffset, int yOffset);

  int GetWidth() const { return _width; }
  int GetHeight() const { return _height; }
  int GetTileWidth() const { return _tileWidth; }
  int GetTileHeight() const { return _tileHeight; }

  MapTile&  GetTile(int x, int y) { return _tileMap[y * _width + x]; }
  void      SetTile(int x, int y, const MapTile& tile) { _tileMap[y * _width + x] = tile; }

private:
  int _width;
  int _height;
  int _tileWidth;
  int _tileHeight;
  MapTile* _tileMap;
};