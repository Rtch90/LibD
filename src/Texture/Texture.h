#pragma once
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif
#define GL_COMPRESSED_RGB 0x84ED
#include <cstdlib>
#include <cstdio>
#include <string>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL_image.h>

typedef struct {
  unsigned char   textureTypeCode;
  short int       textureWidth;
  short int       textureHeight;
  unsigned char   bitCount;
  unsigned char*  textureData;
} TGAFILE;

// Using SDL_image libs.
int BuildTexture(const char* filename, GLuint* texID, GLint param, bool genMips);
int LoadTGAFile(const char* filename, TGAFILE* tgaFile);
int WriteTGAFile(const char* filename, short int width, short int height, unsigned char* textureData);
void BindTexture(GLuint texID);

#include "../System/ResourceManager.h"

class Texture : public Resource {
  template<class T> friend class ResourceManager;

public:
  Texture();
  ~Texture();

  bool Load(const std::string& filename);

  GLuint  GetTexID() const { return texID; }
  int     GetWidth() const { return width; }
  int     GetHeight() const { return height; }

private:
  GLuint texID;
  int width;
  int height;
};

extern ResourceManager<Texture> textureManager;
