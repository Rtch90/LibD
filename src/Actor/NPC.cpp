#include "NPC.h"

NPC::NPC(const Level* level) : Actor(level) {
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
