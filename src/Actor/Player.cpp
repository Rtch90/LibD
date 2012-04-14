#include "Player.h"
#include "../IO/Input.h"

Player::Player(void) : Actor() {
}

Player::~Player(void) {
}

void Player::Update(float dt) {
  Actor::Update(dt);
}

void Player::Render(void) {
  Actor::Render();
}

void Player::Move(float dt) {
  if(KeyStillDown(SDLK_w) || KeyStillDown(SDLK_UP)) {
    y -= _velocity * 60 * dt;
    _actor->SetY(y);
  }
  if(KeyStillDown(SDLK_s) || KeyStillDown(SDLK_DOWN)) {
    y += _velocity * 60 * dt;
    _actor->SetY(y);
  }
  if(KeyStillDown(SDLK_a) || KeyStillDown(SDLK_LEFT)) {
    x -= _velocity * 60 * dt;
    _actor->SetX(x);
  }
  if(KeyStillDown(SDLK_d) || KeyStillDown(SDLK_RIGHT)) {
    x += _velocity * 60 * dt;
    _actor->SetX(x);
  }

  if(KeyDown(SDLK_LSHIFT)) {
    // Run!
    _velocity += 3;
  }
  if(KeyUp(SDLK_LSHIFT)) {
    _velocity -= 3;
  }
}
