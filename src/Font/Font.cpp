#include <SDL/SDL_ttf.h>
#include "../Texture/Texture.h"
#include "../System/Debug.h"
#include "Font.h"

Font::Font(void) {
  _texture = 0;
  _spaceWidth = 0;
  _tabWidth = 0;
  _lineSkip = 0;
  memset(_characters, 0, sizeof(_characters));
  SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

Font::~Font(void) {
  if(_texture) {
    glDeleteTextures(1, &_texture);
    _texture = 0;
  }
}

bool Font::Load(const std::string& filename, int size) {
  TTF_Font* font = TTF_OpenFont(filename.c_str(), size);
  if(!font) {
    Debug::logger->message("Error loading %s: %s", filename.c_str(), TTF_GetError());
    return false;
  }

  glGenTextures(1, &_texture);
  BindTexture(_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Font::TEXTURE_WIDTH, Font::TEXTURE_HEIGHT,
    0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  SDL_Color white = { 0xFF, 0xFF, 0xFF };

  const int ascent = TTF_FontAscent(font);

  int x = 0;
  int y = 0;
  int lineHeight = 0;

  for(int i = 0; i < Font::MAX_CHARS; i++) {
    FontChar& character = _characters[i];

    int minx;
    int maxy;
    TTF_GlyphMetrics(font, i, &minx, NULL, NULL, &maxy, &character.advance);

    character.xOffset += minx;
    character.yOffset -= maxy;
    character.yOffset += ascent;

    SDL_Surface* glyphSurf = TTF_RenderGlyph_Blended(font, i, white);

    if((x + glyphSurf->w) > 255) {
      x = 0;
      y += lineHeight;
      lineHeight = 0;
    }

    if(glyphSurf->w == 0 || glyphSurf->h == 0) {
      SDL_FreeSurface(glyphSurf);
      continue;
    }

    character.uvX = (float)x / (float)Font::TEXTURE_WIDTH;
    character.uvY = (float)y / (float)Font::TEXTURE_HEIGHT;
    character.uvW = (float)glyphSurf->w / (float)Font::TEXTURE_WIDTH;
    character.uvH = (float)glyphSurf->h / (float)Font::TEXTURE_HEIGHT;
    character.width = glyphSurf->w;
    character.height = glyphSurf->h;

    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, character.width, character.height,
      GL_RGBA, GL_UNSIGNED_BYTE, glyphSurf->pixels);

    SDL_FreeSurface(glyphSurf);

    if(character.height > lineHeight) {
      lineHeight = character.height;
    }

    x += character.width;
  }

  TTF_SizeText(font, " ", &_spaceWidth, NULL);
  TTF_SizeText(font, "\t", &_tabWidth, NULL);

  _lineSkip = TTF_FontLineSkip(font);

  TTF_CloseFont(font);

  return true;
}

void Font::RenderText(int xOffset, int yOffset, const char* text) {
  glEnable(GL_TEXTURE_2D);
  BindTexture(_texture);

  glColor4fv(_color);

  glBegin(GL_QUADS);

  const int textLength = strlen(text);
  
  int x = 0;
  int y = 0;

  for(int i = 0; i < textLength; i++) {
    char c = text[i];
   
    if(c == ' ') {
      x += _spaceWidth;
      continue;
    } else if(c == '\t') {
      x += _tabWidth;
      continue;
    } else if(c == '\n') {
      x = 0;
      y += _lineSkip;
      continue;
    }

    FontChar& charInfo = _characters[int(c)];

    glTexCoord2f(
      charInfo.uvX,
      charInfo.uvY);
    glVertex2i(
      charInfo.xOffset + x + xOffset,
      charInfo.yOffset + y + yOffset);
    
    glTexCoord2f(
      charInfo.uvX + charInfo.uvW,
      charInfo.uvY);
    glVertex2i(
      charInfo.xOffset + x + charInfo.width + xOffset,
      charInfo.yOffset + y + yOffset);
    
    glTexCoord2f(
      charInfo.uvX + charInfo.uvW,
      charInfo.uvY + charInfo.uvH);
    glVertex2i(
      charInfo.xOffset + x + charInfo.width + xOffset,
      charInfo.yOffset + y + charInfo.height + yOffset);

    glTexCoord2f(
      charInfo.uvX,
      charInfo.uvY + charInfo.uvH);
    glVertex2i(
      charInfo.xOffset + x + xOffset,
      charInfo.yOffset + y + charInfo.height + yOffset);

    x += charInfo.advance;
  }

  glEnd();

  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Font::TextSize(const char* text, int& width, int& height) {
  width   = 0;
  height  = _lineSkip;
  
  int textLength = strlen(text);
  for(int i = 0; i < textLength; i++) {
    char c = text[i];
    if(c == '\n') {
      height += _lineSkip;
      continue;
    } else if(c == ' ') {
      width += _spaceWidth;
      continue;
    } else if(c == '\t') {
      width += _tabWidth;
      continue;
    }
    width += _characters[(int)c].advance;
  }
}
