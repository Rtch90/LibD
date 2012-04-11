#include "Player.h"

Player::Player(void) {
  PLAYER_SPEED = 15;
}

Player::~Player(void) {
  delete _player->GetTexture();
  delete _player;
}

void Player::Prepare(void) {
  _player = new Sprite();
  _playerTexture = new Texture();
  _playerTexture->Load("../Data/Img/Player.png");
  _player->SetTexture(_playerTexture);
  _player->SetPosition(Vec2(800/2, 600/2));
  _player->SetScale(Vec2(3.0f, 3.0f));
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
