#include "Player.h"

Player::Player(void) {
  PLAYER_SPEED = 15;
  _rotationAngle = 0.0f;
}

Player::~Player(void) {
  delete _player;
}

void Player::Prepare(void) {
  _player = new Sprite();

  _player->LoadSprite("../Data/Img/Player.png");
}

void Player::Render(void) {
  _player->SetRotation(_rotationAngle);
  _player->Draw();
}

void Player::ProcessEvents(void) {
  x = _player->GetX();
  y = _player->GetY();
  if(KeyStillDown(SDLK_w)) {
    y -= PLAYER_SPEED;
    _player->SetY(y);
  }
  if(KeyStillDown(SDLK_s)) {
    y += PLAYER_SPEED;
    _player->SetY(y);
  }
  if(KeyStillDown(SDLK_a)) {
    x -= PLAYER_SPEED;
    _player->SetX(x);
  }
  if(KeyStillDown(SDLK_d)) {
    x += PLAYER_SPEED;
    _player->SetX(x);
  }
}

int Player::GetWidth() { return _player->GetWidth(); }
int Player::GetHeight() { return _player->GetWidth(); }