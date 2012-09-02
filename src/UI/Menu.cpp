#include "Menu.h"
#include "Button.h"
#include "../IO/Input.h"

Menu::Menu(void) {
  _triggeredButton = -1;
  _selectedButton = 0;
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
  // Select first button.
  if(_buttons.empty()) {
    button->SetSelected(true);
  }

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

void Menu::SelectButton(int index) {
  int buttonsIndex = 0;
  for(std::list<Button*>::iterator i = _buttons.begin(); i != _buttons.end(); ++i) {
    (*i)->SetSelected(buttonsIndex == index);
    buttonsIndex++;
  }
  _selectedButton = index;
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

  if(KeyDown(SDLK_DOWN)) {
    _selectedButton++;
    if(_selectedButton == _buttons.size()) {
      _selectedButton = 0;
    }
    SelectButton(_selectedButton);
  }
  else if(KeyDown(SDLK_UP)) {
    _selectedButton--;
    if(_selectedButton < 0) {
      _selectedButton = _buttons.size() - 1;
    }
    SelectButton(_selectedButton);
  }

  if(KeyDown(SDLK_RETURN)) {
    _triggeredButton = _selectedButton;
  }
}

void Menu::Render(void) {
  for(std::list<Button*>::iterator i = _buttons.begin(); i != _buttons.end(); ++i) {
    Button* button = (*i);

    int oldX = button->GetX();
    int oldY = button->GetY();
    button->Render(oldX + x, oldY + y);
    button->SetXY(oldX, oldY);
  }
}
