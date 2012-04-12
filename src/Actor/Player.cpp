#include <stdlib.h>

#include "Player.h"
#include "../Sound/SoundEffect.h"

Player::Player(void) {
  PLAYER_SPEED      = 15;
  _rotationAngle    = 0.0f;
  _allowCollision   = true;
  _notColliding     = false;
  _blueCollision    = false;
  _player           = new Sprite();
  _player->LoadSprite("../Data/Img/Player.png");

  // Loading of sprites and collision details.
  // This should be directed to a collision sheet.
  _collisionBound = new AABB();
  _collisionBound->CreateAABBFromSprite("../Data/Img/Player");

  _environmentCollisionBound = new AABB();
  _environmentCollisionBound->SetMin(_collisionBound->GetMin().x, _collisionBound->GetMax().y - 50.0f);
  _environmentCollisionBound->SetMax(_collisionBound->GetMax().x, _collisionBound->GetMax().y);

  _stepSFX[0] = sfxManager.Load("../Data/SFX/step_cloth1.wav");
  _stepSFX[1] = sfxManager.Load("../Data/SFX/step_cloth2.wav");
  _stepSFX[2] = sfxManager.Load("../Data/SFX/step_cloth3.wav");
  _stepSFX[3] = sfxManager.Load("../Data/SFX/step_cloth4.wav");
  _lastStepSFXPlayed = -1;
}

Player::~Player(void) {
  delete _player;
  delete _collisionBound;
}

void Player::Update(void) {
  // Position and collision bound with the player.
  _collisionBound->SetPositionOffset(_player->GetX(), _player->GetY());
  _environmentCollisionBound->SetPositionOffset(_player->GetPosition().x, _player->GetPosition().y);

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

  //if(_environmentCollisionBound->InCollision())
}

void Player::EntityCollisionTest(void) {

}

void Player::ActorCollisionTest(void) {

}

bool Player::GetInBlueCollision(void) {
  return(_blueCollision && _preventMovement != NONE);
}

void Player::ProcessEvents(void) {
  float oldX = x = _player->GetX();
  float oldY = y = _player->GetY();
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
  if(x != oldX || y != oldY) {
    if(!SoundEffect::IsPlaying(1)) {
      int sfxIndex;
      do {
        sfxIndex = rand() % 4;
      } while(sfxIndex == _lastStepSFXPlayed);

      SoundEffect::Play(_stepSFX[sfxIndex], 1, 0);
      
      _lastStepSFXPlayed = sfxIndex;
    }
  }
}

int Player::GetWidth(void) { return _player->GetWidth(); }
int Player::GetHeight(void) { return _player->GetWidth(); }
