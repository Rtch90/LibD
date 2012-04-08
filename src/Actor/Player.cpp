#include "Player.h"

Player::Player(void) {

}

Player::~Player(void) {
  delete _player->GetTexture();
  delete _player;
}

void Player::Prepare(void) {
  _player = new Sprite();
  _playerTexture = new Texture();
  _playerTexture->Load("../Data/Img/test.png");
  _player->SetTexture(_playerTexture);
  _player->SetPosition(Vec2(800/2, 600/2));
  _player->SetScale(Vec2(4.5f, 4.5f));
}

void Player::Render(void) {
  _player->SetRotation(_rotationAngle);
  _player->Draw();
}

void Player::ProcessEvents(void) {

}
