#include "Player.h"
#include "../IO/Input.h"
#include "../Level/Level.h"
#include "../Level/Warp.h"
#include "../Main/Game.h"

Player::Player(Level* level) : Actor(level) {
  _direction = Actor::RIGHT;
}

Player::~Player(void) {
}

void Player::Update(float dt) {
  Actor::Update(dt);
  
  if(_distanceTraveled > 32.0f) {
    Warp* warp = _level->CheckWarp(x, y, GetMaxWidth(), GetMaxHeight());
    if(warp) {
      _level->GetGame()->Warp(warp->GetTargetMap(), warp->GetTargetX(), warp->GetTargetY());
      _distanceTraveled = 0;
    }
  }
}

void Player::Render(void) {
  Actor::Render();
}

void Player::Move(float dt) {
  if(KeyStillDown(SDLK_a) || KeyStillDown(SDLK_LEFT)) {
    x -= _velocity * 60 * dt;
    _direction = Actor::LEFT;
  }
  if(KeyStillDown(SDLK_d) || KeyStillDown(SDLK_RIGHT)) {
    x += _velocity * 60 * dt;
    _direction = Actor::RIGHT;
  }
  if(KeyStillDown(SDLK_w) || KeyStillDown(SDLK_UP)) {
    y -= _velocity * 60 * dt;
    _direction = Actor::BACK;
  }
  if(KeyStillDown(SDLK_s) || KeyStillDown(SDLK_DOWN)) {
    y += _velocity * 60 * dt;
    _direction = Actor::FRONT;
  }

  if(KeyDown(SDLK_LSHIFT)) {
    _velocity += 3;
  }
  if(KeyUp(SDLK_LSHIFT)) {
    _velocity -= 3;
  }
}
