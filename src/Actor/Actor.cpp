#include <stdlib.h>

#include "Actor.h"
#include "../Sound/SoundEffect.h"
#include "../Level/Level.h"

Actor::Actor(const Level* level) {
  _level = level;

  _stepSFX[0] = sfxManager.Load("../Data/SFX/step_cloth1.wav");
  _stepSFX[1] = sfxManager.Load("../Data/SFX/step_cloth2.wav");
  _stepSFX[2] = sfxManager.Load("../Data/SFX/step_cloth3.wav");
  _stepSFX[3] = sfxManager.Load("../Data/SFX/step_cloth4.wav");
  _lastStepSFXPlayed = -1;

  _velocity        = 4.0f;
  _direction       = FRONT;
  _preventMovement = NONE;

  x = 0.0f;
  y = 0.0f;

  _actorLeft  = new AnimatingSprite();
  _actorRight = new AnimatingSprite();
  _actorFront = new AnimatingSprite();
  _actorBack  = new AnimatingSprite();
}

Actor::~Actor(void) {
  for(int i = 0; i < 4; i++) {
    if(_stepSFX[i]) {
      sfxManager.Destroy(_stepSFX[i]);
      _stepSFX[i] = NULL;
    }
  }
  delete _actorLeft;
  delete _actorRight;
  delete _actorFront;
  delete _actorBack;
}

void Actor::LoadSprites(const String& basename) {
  String frontFilename = String("../Data/Img/") + basename + "/Front/" + basename + "_F";
  String leftFilename = String("../Data/Img/") + basename + "/Left/" + basename + "_L";
  String rightFilename = String("../Data/Img/") + basename + "/Right/" + basename + "_R";
  String backFilename = String("../Data/Img/") + basename + "/Back/" + basename + "_B";

  _actorFront->LoadAnimatingSprite(frontFilename, frontFilename, 4, 1.0f / _velocity);
  _actorLeft->LoadAnimatingSprite(leftFilename, leftFilename, 4, 1.0f / _velocity);
  _actorRight->LoadAnimatingSprite(rightFilename, rightFilename, 4, 1.0f / _velocity);
  _actorBack->LoadAnimatingSprite(backFilename, backFilename, 4, 1.0f / _velocity);
}

void Actor::Update(float dt) {
  GetAnimation()->Update(dt);

  float oldX = x;
  float oldY = y;

  Move(dt);

  if(x != oldX || y != oldY) {
    if(_level->CheckCollision(x, y, GetAnimation()->GetMaxWidth(), GetAnimation()->GetMaxHeight())) {
      x = oldX;
      y = oldY;
      return;
    }
  }

  if(x != oldX || y != oldY) {
    GetAnimation()->SetCurrentAnimation(1);

    if(!SoundEffect::IsPlaying(1)) {
      int sfxIndex;
      do {
        sfxIndex = rand() % 4;
      } while(sfxIndex == _lastStepSFXPlayed);

      SoundEffect::Play(_stepSFX[sfxIndex], 1, 0);

      _lastStepSFXPlayed = sfxIndex;
    }
  }
  else {
    GetAnimation()->SetCurrentAnimation(0);
  }
}

void Actor::Render(void) {
  GetAnimation()->Render(x, y);
}

void Actor::Render(float x, float y) {
  GetAnimation()->Render(x, y);
}

AnimatingSprite* Actor::GetAnimation(void) {
  if(_direction == LEFT) {
    return _actorLeft;
  }
  else if(_direction == RIGHT) {
    return _actorRight;
  }
  else if(_direction == FRONT) {
    return _actorFront;
  }
  else if(_direction == BACK) {
    return _actorBack;
  }
  return NULL;
}

float Actor::GetWidth(void) {
  return GetAnimation()->GetCurrentFrameSprite()->GetWidth();
}

float Actor::GetHeight(void) {
  return GetAnimation()->GetCurrentFrameSprite()->GetHeight();
}

float Actor::GetMaxWidth(void) {
  return GetAnimation()->GetMaxWidth();
}

float Actor::GetMaxHeight(void) {
  return GetAnimation()->GetMaxHeight();
}
