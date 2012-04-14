#pragma once

#include "../System/ResourceManager.h"

class Texture;

struct FontChar {
  float uvX;
  float uvY;
  float uvW;
  float uvH;
  int width;
  int height;
  int xOffset;
  int yOffset;
  int advance;
};

class Font : public Resource {
  template<class T> friend class ResourceManager;

public:
  Font(void);
  ~Font(void);

  bool Load(const std::string& filename);

  void DrawText(int xOffset, int yOffset, const char* text);

  int     GetLineSkip() const { return _lineSkip; }
  float*  GetColor() { return _color; }

  void SetColor(float r, float g, float b, float a) {
    _color[0] = r;
    _color[1] = g;
    _color[2] = b;
    _color[3] = a;
  }

private:
  unsigned int _texture;
  int _spaceWidth;
  int _tabWidth;
  int _lineSkip;

  float _color[4];

  static const int MAX_CHARS = 128;
  static const int TEXTURE_WIDTH = 256;
  static const int TEXTURE_HEIGHT = 256;

  FontChar _characters[MAX_CHARS];
};
