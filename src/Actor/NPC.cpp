#include "NPC.h"

NPC::NPC(void) : Actor() {
  LoadSprite("../Data/Img/Player.png");
}

NPC::~NPC(void) {
}

void NPC::Update(float dt) {
  Actor::Update(dt);
}

void NPC::Render(void) {
  Actor::Render(105, 125);
}

void NPC::Move(float dt) {
}
