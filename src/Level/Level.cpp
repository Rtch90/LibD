#include <map>
#include <algorithm>
#include <string.h>
#include "Level.h"
#include "Layer.h"
#include "Tileset.h"
#include "Warp.h"
#include "../Sound/Music.h"
#include "../System/Debug.h"
#include "../TMXParser/Tmx.h"
#include "../Actor/NPC.h"
#include "../Math/Rect.h"

#ifdef _WIN32
#ifndef strcasecmp
#define strcasecmp stricmp
#endif
#ifndef strncasecmp
#define strncasecmp strnicmp
#endif
#endif

using saracraft::util::Debug;

Level::Level(Game* game) {
  _game = game;
  _width = 0;
  _height = 0;
  _tileWidth = 0;
  _tileHeight = 0;
  _bgm = NULL;
  _collisions = NULL;
  _middleLayer = -1;
}

Level::~Level() {
  for(int i = 0; i < _layers.size(); i++) {
    delete _layers.at(i);
  }
  _layers.clear();

  for(std::list<Tileset*>::iterator i = _tilesets.begin(); i != _tilesets.end(); ++i) {
    delete (*i);
  }
  _tilesets.clear();

  for(int i = 0; i < _npcs.size(); i++) {
    delete _npcs.at(i);
  }
  _npcs.clear();

  for(std::list<Warp*>::iterator i = _warps.begin(); i != _warps.end(); ++i) {
    delete (*i);
  }
  _warps.clear();

  if(_collisions) {
    delete[] _collisions;
    _collisions = NULL;
  }

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

  _collisions = new bool[_width * _height];
  for(int i = 0; i < (_width * _height); i++) {
    _collisions[i] = false;
  }

  for(int i = 0; i < map.GetNumLayers(); i++) {
    const Tmx::Layer* tmxLayer = map.GetLayer(i);

    if(!strcasecmp(tmxLayer->GetName().c_str(), "collision")) {
      for(int x = 0; x < _width; x++) {
        for(int y = 0; y < _height; y++) {
          Tmx::MapTile tile = tmxLayer->GetTile(x, y);
          _collisions[y * _width + x] = tile.tilesetId > -1;
        }
      }
      continue;
    }
    else if(!strcasecmp(tmxLayer->GetName().c_str(), "middle")) {
      _middleLayer = i;
    }

    Layer* layer = new Layer(
      tmxLayer->GetWidth(), tmxLayer->GetHeight(),
      _tileWidth, _tileHeight);

    for(int x = 0; x < layer->GetWidth(); x++) {
      for(int y = 0; y < layer->GetHeight(); y++) {
        Tmx::MapTile tmxTile = tmxLayer->GetTile(x, y);

        MapTile tile;
        if(tmxTile.tilesetId != -1) {
          const Tmx::Tileset* tmxTileset = map.GetTileset(tmxTile.tilesetId);
          tile.id = tmxTile.id;
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

  if(_middleLayer == -1) {
    _middleLayer = int(floor(float(_layers.size()) / 2.0f)); // <-- nasty
  }

  for(int i = 0; i < map.GetNumObjectGroups(); i++) {
    const Tmx::ObjectGroup* tmxGroup = map.GetObjectGroup(i);
    for(int j = 0; j < tmxGroup->GetNumObjects(); j++) {
      const Tmx::Object* tmxObject = tmxGroup->GetObject(j);
      if(!strncasecmp(tmxObject->GetName().c_str(), "NPC", 3)) {
        NPC* npc = new NPC(this);
        npc->LoadSprites(tmxObject->GetProperties().GetLiteralProperty("image").c_str());
        npc->SetXY(tmxObject->GetX(), tmxObject->GetY());
        _npcs.push_back(npc);
      }
      else if(!strncasecmp(tmxObject->GetName().c_str(), "Warp", 4)) {
        Warp* warp = new Warp();
        warp->SetXY(tmxObject->GetX(), tmxObject->GetY());
        warp->SetWidthHeight(tmxObject->GetWidth(), tmxObject->GetHeight());
        warp->SetTargetMap(tmxObject->GetProperties().GetLiteralProperty("map").c_str());
        warp->SetTargetX(tmxObject->GetProperties().GetNumericProperty("x") * 32);
        warp->SetTargetY(tmxObject->GetProperties().GetNumericProperty("y") * 32);
        _warps.push_back(warp);
      }
    }
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
  for(int i = 0; i < _layers.size(); i++) {
    _layers.at(i)->Update(dt);
  }
  for(int i = 0; i < _npcs.size(); i++) {
    _npcs.at(i)->Update(dt);
  }
}

void Level::DrawBackground(int xOffset, int yOffset, float playerY) {
  for(int i = 0; i < (_middleLayer + 1); i++) {
    _layers.at(i)->Draw(xOffset, yOffset);
  }
  for(int i = 0; i < _npcs.size(); i++) {
    NPC* npc = _npcs.at(i);
    if(npc->GetY() <= playerY)
      npc->Render();
  }
}

void Level::DrawForeground(int xOffset, int yOffset, float playerY) {
  for(int i = (_middleLayer + 1); i < _layers.size(); i++) {
    _layers.at(i)->Draw(xOffset, yOffset);
  }
  for(int i = 0; i < _npcs.size(); i++) {
    NPC* npc = _npcs.at(i);
    if(npc->GetY() > playerY)
      npc->Render();
  }
}

bool Level::CheckCollision(float x, float y, float w, float h) const {
  if(x < 0.0f || x >= (float)((_width - 1) * _tileWidth) ||
     y < 0.0f || y >= (float)((_height - 1) * _tileHeight)) {
    return true;
  }

  int tileX;
  int tileY;

  // Check Top Left
  tileX = (int)(x / (float)_tileWidth);
  tileY = (int)(y / (float)_tileHeight);
  if(tileX >= 0 && tileX < _width &&
     tileY >= 0 && tileY < _height) {
    if(_collisions[tileY * _width + tileX]) {
      return true;
    }
  }

  // Check Top Right
  tileX = (int)((x + w) / (float)_tileWidth);
  tileY = (int)(y / (float)_tileHeight);
  if(tileX >= 0 && tileX < _width &&
     tileY >= 0 && tileY < _height) {
    if(_collisions[tileY * _width + tileX]) {
      return true;
    }
  }

  // Check Bottom Right
  tileX = (int)((x + w) / (float)_tileWidth);
  tileY = (int)((y + h) / (float)_tileHeight);
  if(tileX >= 0 && tileX < _width &&
     tileY >= 0 && tileY < _height) {
    if(_collisions[tileY * _width + tileX]) {
      return true;
    }
  }

   // Check Bottom Left
  tileX = (int)(x / (float)_tileWidth);
  tileY = (int)((y + h) / (float)_tileHeight);
  if(tileX >= 0 && tileX < _width &&
     tileY >= 0 && tileY < _height) {
    if(_collisions[tileY * _width + tileX]) {
      return true;
    }
  }

  return false;
}

Warp* Level::CheckWarp(float x, float y, float w, float h) const {
  Rect objectArea(x, y, w, h);
  for(std::list<Warp*>::const_iterator i = _warps.begin(); i != _warps.end(); ++i) {
    Warp* warp = (*i);
    Rect warpArea(
      warp->GetX(), warp->GetY(),
      warp->GetWidth(), warp->GetHeight());
    if(warpArea.CollidesWith(objectArea)) {
      return warp;
    }
  }
  return NULL;
}
