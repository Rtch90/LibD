#include "Tileset.h"
#include "../Sprite/Sprite.h"

Tileset::Tileset(int tileWidth, int tileHeight) {
  _tileWidth = tileWidth;
  _tileHeight = tileHeight;
  _sprite = NULL;
  _widthInTiles = 0;
}

Tileset::~Tileset() {
}

bool Tileset::LoadImage(const std::string& filename) {
  if(_sprite) {
    delete _sprite;
  }

  _sprite = new Sprite();
  
  if (!_sprite->LoadSprite(filename)) {
    delete _sprite;
    return false;
  }

  _widthInTiles = (int)_sprite->GetWidth() / _tileWidth;

  return true;
}

void Tileset::DrawTile(int id, const Vec2& position) {
  _sprite->SetPosition(position);
  
  Rect sourceRect(
    float(id % _widthInTiles) * _tileWidth,
    float(id / _widthInTiles) * _tileHeight,
    (float)_tileWidth,
    (float)_tileHeight);

  _sprite->DrawRegion(sourceRect);
}
