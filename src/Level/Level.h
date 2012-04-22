#pragma once 

#include <string>
#include <list>

class Layer;
class Tileset;
class Music;
class NPC;

class Level {
public:
  Level();
  ~Level();

  bool Load(const std::string& filename);

  void PlayBGM();

  void Update(float dt);
  void Draw(int xOffset, int yOffset);

  int GetWidth() const { return _width; }
  int GetHeight() const { return _height; }
  int GetTileWidth() const { return _tileWidth; }
  int GetTileHeight() const { return _tileHeight; }

  bool CheckCollision(float x, float y, float w, float h) const;

private:
  int _width;
  int _height;
  int _tileWidth;
  int _tileHeight;
  std::list<Layer*> _layers;
  std::list<Tileset*> _tilesets;
  std::list<NPC*> _npcs;
  Music* _bgm;
  bool* _collisions;
};
