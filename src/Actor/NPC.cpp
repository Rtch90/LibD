#include "NPC.h"

NPC::NPC(void) : Actor() {
}

NPC::~NPC(void) {
}

void NPC::Update(float dt) {
  Actor::Update(dt);
}

void NPC::Render(void) {
  Actor::Render();
}

void NPC::Move(float dt) {
}
