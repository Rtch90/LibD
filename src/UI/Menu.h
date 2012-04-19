#pragma once

#include <list>

class Button;

class Menu {
public:
  enum {
    ALIGN_HORIZONTALLY,
    ALIGN_VERTICALLY
  };

  Menu(void);
  ~Menu(void);

  void AddButton(Button* button);
  void AlignButtons(int how);
  void SelectButton(int index);

  void Update(void);
  void Render(void);

  int GetTriggeredButton() const { return _triggeredButton; }

  int   GetX(void) const    { return x; }
  int   GetY(void) const    { return y; }
  void  SetX(int x)         { this->x = x; }
  void  SetY(int y)         { this->y = y; }
  void  SetXY(int x, int y) { SetX(x); SetY(y); }

private:
  std::list<Button*> _buttons;
  int _triggeredButton;
  int _selectedButton;
  
  int x;
  int y;
};