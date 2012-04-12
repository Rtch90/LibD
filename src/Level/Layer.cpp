#include "Layer.h"
#include "Tileset.h"
#include "../Global/Constants.h"

Layer::Layer(int width, int height, int tileWidth, int tileHeight) {
  _width = width;
  _height = height;
  _tileWidth = tileWidth;
  _tileHeight = tileHeight;
  _tileMap = new MapTile[width * height];
}

Layer::~Layer() {
  delete[] _tileMap;
}

void Layer::Update(float dt) {
  for(int x = 0; x < _width; x++) {
    for(int y = 0; y < _height; y++) {    
    }
  }
}

void Layer::Draw(int xOffset, int yOffset) {
  int xOffsetTiles = xOffset / _tileWidth;
  int yOffsetTiles = yOffset / _tileHeight;

  int minX = std::max(xOffsetTiles, 0);
  int maxX = std::min(_width, xOffsetTiles + WINDOW_WIDTH / (int)_tileWidth + 2);
  int minY = std::max(yOffsetTiles, 0);
  int maxY = std::min(_height, yOffsetTiles + WINDOW_HEIGHT / (int)_tileHeight + 2);
  
  for(int x = minX; x < maxX; x++) {
    for(int y = minY; y < maxY; y++) {
      MapTile& tile = GetTile(x, y);
      if(tile.tileset != NULL) {
        tile.tileset->DrawTile(tile.id, Vec2((float)x * _tileWidth, (float)y * _tileHeight));
      }
    }
  }
}


