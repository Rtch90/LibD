#pragma once

#include "../System/String.h"
#include "../Font/Font.h"

class Button {
public:
  Button(void);

  void Update(void);
  void Render(void);

  Font*   GetFont(void) { return _font; }
  void    SetFont(Font* font);
  
  const String& GetText(void) const { return _text;}
  void          SetText(const String& text);
  
  bool IsHighlighted(void) const        { return _highlighted; }
  void SetHighlighted(bool highlighted) { _highlighted = highlighted; }

  bool Triggered(void) const { return _triggered; }

  int   GetX(void) const    { return x; }
  int   GetY(void) const    { return y; }
  void  SetX(int x)         { this->x = x; }
  void  SetY(int y)         { this->y = y; }
  void  SetXY(int x, int y) { SetX(x); SetY(y); }

  int GetWidth(void) const { return w; }
  int GetHeight(void) const { return h; }

private:
  Font*   _font;
  String  _text;
 
  bool _highlighted;
  bool _triggered;

  int x;
  int y;
  int w;
  int h;
};
