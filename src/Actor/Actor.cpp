#include "Actor.h"

Actor::Actor(void) : VELOCITY(10.0f) {
  _actor = new Sprite();
}

Actor::~Actor(void) {
  delete _actor;
}

void Actor::LoadSprite(const char* filename, float w, float h) {

}

void Actor::Update(void) {

}

void Actor::Render(void) {
  _actor->Draw();
}
