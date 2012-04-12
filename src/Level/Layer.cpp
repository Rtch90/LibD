#include "Layer.h"
#include "Tileset.h"

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

void Layer::Draw() {
  for(int x = 0; x < _width; x++) {
    for(int y = 0; y < _height; y++) {
      MapTile& tile = GetTile(x, y);
      tile.tileset->DrawTile(tile.id, Vec2((float)x * _tileWidth, (float)y * _tileHeight));
    }
  }
}


