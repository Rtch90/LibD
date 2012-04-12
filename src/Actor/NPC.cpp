#include "NPC.h"

NPC::NPC(void) {
  _NPC = new Sprite();
}

NPC::~NPC(void) {
  delete _NPC;
}

void NPC::Update(void) {

}

void NPC::Render(void) {
  _NPC->Draw();
}
