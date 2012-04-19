#include "Menu.h"
#include "Button.h"

Menu::Menu(void) {
  _triggeredButton = -1;
  x = 0;
  y = 0;
}

Menu::~Menu(void) {
  for(std::list<Button*>::iterator i = _buttons.begin(); i != _buttons.end(); ++i) {
    delete (*i);
  }
  _buttons.clear();
}

void Menu::AddButton(Button* button) {
  _buttons.push_back(button);
}

void Menu::AlignButtons(int how) {
  int x = 0;
  int y = 0;
  for(std::list<Button*>::iterator i = _buttons.begin(); i != _buttons.end(); ++i) {   
    Button* button = (*i);
    button->SetXY(x, y);

    if(how == Menu::ALIGN_HORIZONTALLY) {
      x += button->GetWidth() + 24;
    } else if(how == Menu::ALIGN_VERTICALLY) {
      y += button->GetHeight() + 2;
    }
  }
}

void Menu::Update(void) {
  _triggeredButton = -1;

  int index = 0;
  for(std::list<Button*>::iterator i = _buttons.begin(); i != _buttons.end(); ++i) {
    Button* button = (*i);
    
    int oldX = button->GetX();
    int oldY = button->GetY();
    button->SetXY(oldX + x, oldY + y);
    button->Update();
    button->SetXY(oldX, oldY);

    if(button->Triggered()) {
      _triggeredButton = index;
    }

    index++;
  }
}

void Menu::Render(void) {
  for(std::list<Button*>::iterator i = _buttons.begin(); i != _buttons.end(); ++i) {
    Button* button = (*i);
    
    int oldX = button->GetX();
    int oldY = button->GetY();
    button->SetXY(oldX + x, oldY + y);
    button->Render();
    button->SetXY(oldX, oldY);
  }
}
