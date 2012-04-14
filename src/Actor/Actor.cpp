#include <stdlib.h>

#include "Actor.h"
#include "../Sound/SoundEffect.h"

Actor::Actor(void) {
  _actor = new Sprite();

  _stepSFX[0] = sfxManager.Load("../Data/SFX/step_cloth1.wav");
  _stepSFX[1] = sfxManager.Load("../Data/SFX/step_cloth2.wav");
  _stepSFX[2] = sfxManager.Load("../Data/SFX/step_cloth3.wav");
  _stepSFX[3] = sfxManager.Load("../Data/SFX/step_cloth4.wav");
  _lastStepSFXPlayed = -1;

  _velocity = 4.0f;
}

Actor::~Actor(void) {
  for(int i = 0; i < 4; i++) {
    if(_stepSFX[i]) {
      sfxManager.Destroy(_stepSFX[i]);
      _stepSFX[i] = NULL;
    }
  }
  delete _actor;
}

void Actor::LoadSprite(const char* filename) {
  _actor->LoadSprite(filename);
  w = _actor->GetWidth();
  h = _actor->GetHeight();
}

void Actor::Update(float dt) {
  float oldX = x = _actor->GetX();
  float oldY = y = _actor->GetY();

  Move(dt);

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

void Actor::Render(void) {
  _actor->Draw();
}
