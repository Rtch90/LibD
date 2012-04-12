#include "Player.h"

Player::Player(void) {
  PLAYER_SPEED      = 15;
  _rotationAngle    = 0.0f;
  _allowCollision   = true;
  _notColliding     = false;
  _blueCollision    = false;

  // Loading of sprites and collision details.
  _player->LoadSprite("../Data/Img/Player.png");
  // This should be directed to a collision sheet.
  _collisionBound = new AABB();
  _collisionBound->CreateAABBFromSprite("../Data/Img/Player.png");

  _environmentCollisionBound = new AABB();
  _environmentCollisionBound->SetMin(_collisionBound->GetMin().x, _collisionBound->GetMax().y - 50.0f);
  _environmentCollisionBound->SetMax(_collisionBound->GetMax());
}

Player::~Player(void) {
  delete _player;
  delete _collisionBound;
}

void Player::Update(void) {
  // Position and collision bound with the player.
  _collisionBound->SetPositionOffset(_player->GetX(), _player->GetY());
  _environmentCollisionBound->SetPositionOffset(_player->GetX, _player->GetY());

  // Time to process the collisions.
  ProcessCollisions();

  // Process events here.
  ProcessEvents();
}

void Player::Render(void) {
  _player->SetRotation(_rotationAngle);
  _player->Draw();
}

void Player::ProcessCollisions(void) {
  // Process collisions with entities and actors.
  // We should ensure we are not dead.
  EntityCollisionTest();
  ActorCollisionTest();

  // Set all collision flags to false conditions
  // then they will need to be proven in the test.
  _notColliding   = true;
  _blueCollision  = false;
  bool onFloor    = false;

  // This is going to get messy, and I am going to have to play
  // in KonoM's level stuff.

  // We need a level manager class that will create a list of collidable
  // entites/actors.

  // I'll do this tomorrow now.
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
