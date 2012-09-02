#include "Slot.h"

Slot::Slot(void) {
  _triggered  = false;
  _selected   = false;
  x = 0;
  y = 0;
  w = 0;
  h = 0;

  //Sprite* _slot = new Sprite();
  //_slot->LoadSprite("../Data/Img/HUD/SpellHud.png");
}

Slot::~Slot(void) {
  //delete _slot;
  delete  _slotArray[_index];
}

void Slot::PrepareSlot(void) {
  for(int i = 0; i < MAX_SLOTS; i++) {
    _slotArray[i] = new Button();
    _slotArray[i]->LoadSprite("../Data/Img/HUD/SpellHud.png");
    _index = i;
  }
}

void Slot::Render(void) {
  //_slot->Draw();
}

void Slot::Render(float x, float y) {
  for(int i = 0; i < _index; i++) {
    int offset = 52;

    if(i != _index) {
      x += offset;
      _slotArray[_index]->Render(x, y);
    }
  }
}

void Slot::ProcessEvents(void) {
  _slotArray[0]->Update();
  if(_slotArray[0]->IsSelected()) {
    if(MouseUp(SDL_BUTTON(1))) {
      _slotArray[0]->SetSelected(true);
      Debug::logger->message("Slot has been triggered.");
    }
  } else {
    //Debug::logger->message("Not triggered");
  }
}
