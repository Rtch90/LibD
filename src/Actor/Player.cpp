#include <stdlib.h>

#include "Player.h"
#include "../Sound/SoundEffect.h"

Player::Player(void) {
  PLAYER_SPEED      = 15;
  _rotationAngle    = 0.0f;
  _player           = new Sprite();
  _player->LoadSprite("../Data/Img/Player.png");

  _stepSFX[0] = sfxManager.Load("../Data/SFX/step_cloth1.wav");
  _stepSFX[1] = sfxManager.Load("../Data/SFX/step_cloth2.wav");
  _stepSFX[2] = sfxManager.Load("../Data/SFX/step_cloth3.wav");
  _stepSFX[3] = sfxManager.Load("../Data/SFX/step_cloth4.wav");
  _lastStepSFXPlayed = -1;
}

Player::~Player(void) {
  delete _player;
}

void Player::Update(void) {
  // Process events here.
  ProcessEvents();
}

void Player::Render(void) {
  _player->SetRotation(_rotationAngle);
  _player->Draw();
}

void Player::ProcessEvents(void) {
  float oldX = x = _player->GetX();
  float oldY = y = _player->GetY();
  if(KeyStillDown(SDLK_w) || KeyStillDown(SDLK_UP)) {
    y -= PLAYER_SPEED;
    _player->SetY(y);
  }
  if(KeyStillDown(SDLK_s) || KeyStillDown(SDLK_DOWN)) {
    y += PLAYER_SPEED;
    _player->SetY(y);
  }
  if(KeyStillDown(SDLK_a) || KeyStillDown(SDLK_LEFT)) {
    x -= PLAYER_SPEED;
    _player->SetX(x);
  }
  if(KeyStillDown(SDLK_d) || KeyStillDown(SDLK_RIGHT)) {
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
