#pragma once 

#include <string>
#include <list>

class Layer;
class Tileset;

class Level {
public:
  Level();
  ~Level();

  bool Load(const std::string& filename);

  void Update(float dt);
  void Draw();

  int GetWidth() const { return _width; }
  int GetHeight() const { return _height; }

private:
  int _width;
  int _height;
  int _tileWidth;
  int _tileHeight;
  std::list<Layer*> _layers;
  std::list<Tileset*> _tilesets;
};
