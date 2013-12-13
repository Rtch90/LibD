#pragma once
#include "../Math/Vec2.h"
#include "../System/Debug.h"
#include "../Global/Globals.h"
#include "../UI/Button.h"
#include "../IO/Input.h"
#define MAX_SLOTS 15

class Slot {
public:
  Slot(void);
  ~Slot(void);

  void PrepareSlot(void);
  void Render(void);
  void Render(float x, float y);

  void ProcessEvents(void);

  bool IsSelected(void) const     { return _selected; }
  void SetSelected(bool selected) { _selected = selected; }
  bool Triggered(void) const      { return _triggered; }

  int GetX(void)                  { return x; }
  int GetY(void)                  { return y; }
  void SetX(int x)                { this->x = x; }
  void SetY(int y)                { this->y = y; }
  void SetXY(int x, int y)        { SetX(x); SetY(y); }

  int GetWidth(void)              { return w; }
  int GetHeight(void)             { return h; }


private:
  //Button* _slot;
  Button* _slotArray[MAX_SLOTS];
  int _index;

  bool _selected;
  bool _triggered;

  int x,y,w,h;
};
