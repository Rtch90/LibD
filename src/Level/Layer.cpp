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
  int xOffsetTiles = xOffset / _width;
  int yOffsetTiles = yOffset / _height;

  int minX = std::max(0, xOffsetTiles - 1);
  int maxX = std::min(_width, xOffsetTiles + (WINDOW_WIDTH / (int)_tileWidth) + 1);
  int minY = std::max(0, yOffsetTiles - 1);
  int maxY = std::min(_height, yOffsetTiles + (WINDOW_HEIGHT / (int)_tileHeight) + 1);
  
  for(int x = 0; x < _width; x++) {
    for(int y = 0; y < _height; y++) {
      MapTile& tile = GetTile(x, y);
      if(tile.tileset != NULL) {
        tile.tileset->DrawTile(tile.id, Vec2((float)x * _tileWidth, (float)y * _tileHeight));
      }
    }
  }
}


