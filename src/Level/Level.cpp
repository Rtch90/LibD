#include <map>
#include "Level.h"
#include "Layer.h"
#include "Tileset.h"
#include "../System/Debug.h"
#include "../TMXParser/Tmx.h"

Level::Level() {
  _width = 0;
  _height = 0;
  _tileWidth = 0;
  _tileHeight = 0;
}

Level::~Level() {
  for(std::list<Layer*>::iterator i = _layers.begin(); i != _layers.end(); ++i) {
    delete (*i);
  }
  _layers.clear();

  for(std::list<Tileset*>::iterator i = _tilesets.begin(); i != _tilesets.end(); ++i) {
    delete (*i);
  }
  _tilesets.clear();
}

bool Level::Load(const std::string& filename) {
  Tmx::Map map;
  map.ParseFile(filename);

  if(map.HasError()) {
    Debug::logger->message("Error while loading level %s: %s\n", filename.c_str(), map.GetErrorText().c_str());
    return false;
  }

  _width = map.GetWidth();
  _height = map.GetHeight();
  _tileWidth = map.GetTileWidth();
  _tileHeight = map.GetTileHeight();

  std::map<const Tmx::Tileset*, Tileset*> tilesetMap;

  for(int i = 0; i < map.GetNumTilesets(); i++) {
    const Tmx::Tileset* tmxTileset = map.GetTileset(i);

    Tileset* tileset = new Tileset(_tileWidth, _tileHeight);
    tileset->LoadImage(map.GetFilepath() + tmxTileset->GetImage()->GetSource());

    _tilesets.push_back(tileset);

    tilesetMap.insert(std::pair<const Tmx::Tileset*, Tileset*>(tmxTileset, tileset));
  }

  for(int i = 0; i < map.GetNumLayers(); i++) {
    const Tmx::Layer* tmxLayer = map.GetLayer(i);
    
    Layer* layer = new Layer(
      tmxLayer->GetWidth(), tmxLayer->GetHeight(),
      _tileWidth, _tileHeight);

    for(int x = 0; x < layer->GetWidth(); x++) {
      for(int y = 0; y < layer->GetHeight(); y++) {
        Tmx::MapTile tmxTile = tmxLayer->GetTile(x, y);
        
        const Tmx::Tileset* tmxTileset = map.FindTileset(tmxTile.gid);

        MapTile tile;
        tile.id = tmxTile.gid - tmxTileset->GetFirstGid();
        tile.tileset = tilesetMap.find(tmxTileset)->second;

        layer->SetTile(x, y, tile);
      }
    }
    
    _layers.push_back(layer);
  }

  return true;
}

void Level::Draw() {
  for(std::list<Layer*>::iterator i = _layers.begin(); i != _layers.end(); ++i) {
    (*i)->Draw();
  }
}
