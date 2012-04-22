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

Level::Level(Game* game) {
  _game = game;
  _width = 0;
  _height = 0;
  _tileWidth = 0;
  _tileHeight = 0;
  _bgm = NULL;
  _collisions = NULL;
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

  for(std::list<NPC*>::iterator  i = _npcs.begin(); i != _npcs.end(); ++i) {
    delete (*i);
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
          _collisions[y * _width + x] = tile.gid != 0;
        }
      }
      continue;
    }

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
  for(std::list<Layer*>::iterator i = _layers.begin(); i != _layers.end(); ++i) {
    (*i)->Update(dt);
  }
  for(std::list<NPC*>::iterator i = _npcs.begin(); i != _npcs.end(); ++i) {
    (*i)->Update(dt);
  }
}

void Level::Draw(int xOffset, int yOffset) {
  for(std::list<Layer*>::iterator i = _layers.begin(); i != _layers.end(); ++i) {
    (*i)->Draw(xOffset, yOffset);
  }
  for(std::list<NPC*>::iterator i = _npcs.begin(); i != _npcs.end(); ++i) {
    (*i)->Render();
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
