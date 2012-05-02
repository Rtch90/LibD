#pragma once 

#include <string>
#include <vector>
#include <list>

class Game;
class Layer;
class Tileset;
class Music;
class NPC;
class Warp;

class Level {
public:
  Level(Game* game);
  ~Level(void);

  bool Load(const std::string& filename);

  void PlayBGM(void);

  void Update(float dt);
  void DrawBackground(int xOffset, int yOffset, float playerY);
  void DrawForeground(int xOffset, int yOffset, float playerY);

  Game* GetGame() { return _game; }

  int GetWidth() const { return _width; }
  int GetHeight() const { return _height; }
  int GetTileWidth() const { return _tileWidth; }
  int GetTileHeight() const { return _tileHeight; }

  bool  CheckCollision(float x, float y, float w, float h) const;
  Warp* CheckWarp(float x, float y, float w, float h) const;

private:
  Game* _game;
  int _width;
  int _height;
  int _tileWidth;
  int _tileHeight;
  std::vector<Layer*> _layers;
  std::list<Tileset*> _tilesets;
  std::vector<NPC*> _npcs;
  std::list<Warp*> _warps;
  Music* _bgm;
  bool* _collisions;
  int _middleLayer;
};
