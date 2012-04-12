#include <map>
#include <algorithm>
#include "Level.h"
#include "Layer.h"
#include "Tileset.h"
#include "../Sound/Music.h"
#include "../System/Debug.h"
#include "../TMXParser/Tmx.h"

Level::Level() {
  _width = 0;
  _height = 0;
  _tileWidth = 0;
  _tileHeight = 0;
  _bgm = NULL;
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

  if(_bgm) {
    musicManager.Destroy(_bgm);
    _bgm = NULL;
  }
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
        if(tmxTile.gid != 0) {
          tile.id = tmxTile.gid - tmxTileset->GetFirstGid();
          tile.tileset = tilesetMap.find(tmxTileset)->second;
        } else {
          tile.id = 0;
          tile.tileset = NULL;
        }
        layer->SetTile(x, y, tile);
      }
    }
    
    _layers.push_back(layer);
  }

  std::map<std::string, std::string> mapProps = map.GetProperties().GetList();
  for(std::map<std::string, std::string>::iterator i = mapProps.begin(); i != mapProps.end(); ++i) {
    if(i->first == "BGM") {
      _bgm = musicManager.Load(i->second);
    }
  }

  return true;
}

void Level::PlayBGM() {
  if(_bgm) {
    Music::Play(_bgm, -1);
  }
}

void Level::Update(float dt) {
  for(std::list<Layer*>::iterator i = _layers.begin(); i != _layers.end(); ++i) {
    (*i)->Update(dt);
  }
}

void Level::Draw(int xOffset, int yOffset) {
  for(std::list<Layer*>::iterator i = _layers.begin(); i != _layers.end(); ++i) {
    (*i)->Draw(xOffset, yOffset);
  }
}
