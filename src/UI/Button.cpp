#include "Button.h"
#include "../Font/Font.h"
#include "../IO/Input.h"

Button::Button(void) {
  _text = "";
  _font = NULL;
  _highlighted = false;
  _selected = false;
  _triggered = false;
  x = 0;
  y = 0;
  w = 0;
  h = 0;
}

void Button::Update(void) {
  _triggered = false;
  
  int mouseX = ::GetX();
  int mouseY = ::GetY();

  if((mouseX >= x) && (mouseX < (x + w)) &&
     (mouseY >= y) && (mouseY < (y + h)))
  {
    _highlighted = true;
    if(MouseUp(SDL_BUTTON(1))) {
      _triggered = true;
    }
  } else {
    _highlighted = false;
  }
}

void Button::Render(void) {
  if(_font) {
    if(_highlighted || _selected) {
      _font->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    } else {
      _font->SetColor(0.5f, 0.5f, 0.5f, 1.0f);
    }
    _font->RenderText(x, y, _text.GetPointer());
    _font->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
  }
}

void Button::SetFont(Font* font) {
  _font = font;
  if(_text.Length() > 0) {
    font->TextSize(_text.GetPointer(), w, h);
  }
}

void Button::SetText(const String& text) {
  _text = text;
  if((_text.Length() > 0) && _font) {
    _font->TextSize(text.GetPointer(), w, h);
  }
}
