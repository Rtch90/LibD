#include "AnimatingSprite.h"

// Stores and controls a number of sprites that are
// used to create an animation. Each frame is saved as an
// individual sprite, then the update method controls the frame
// that should be displayed.

AnimatingSprite::AnimatingSprite(void) {
  _spriteCounter = 0;
  _timer         = 0;
  _currentFrame  = 1;
}

AnimatingSprite::~AnimatingSprite(void) {
  for(int i = 0; i < _numberOfFrames; i++) {
    delete _sprites[i];
  }
}

void AnimatingSprite::Render(void) {
  _sprites[_currentFrame-1]->Draw();
}

void AnimatingSprite::Render(float x, float y) {
  // Render with a position.
  _sprites[_currentFrame-1]->Draw(x, y);
}

void AnimatingSprite::Update(float dt) {
  /*
   * Use time to control the current animation frame.
   * First off, check to see if there is an animation sequence,
   * then if the dt is greate than the set animation speed,
   * the _currentFrame is set to the next valid frame.
   */

  if(_sequence) {
    _timer += dt;
    if(_timer > _animationSpeed) {
      _timer = 0;
      _currentFrame++;
      Animation* curAnim = _sequence->GetAnimation(_currentAnimation);
      if(_currentFrame > curAnim->frameEnd) {
        _currentFrame = curAnim->_loopTo;
      }
    }
  }
}


void AnimatingSprite::LoadAnimatingSprite(const char* filename, const char* sequence, int frames, float animationSpeed) {
  _maxWidth = 0;
  _maxHeight = 0;
  for(int i = 0; i < frames; i++) {
    String tempFilename;
    tempFilename = "";
    if(i < 10) {
      tempFilename.Format("%s.00%i.png", filename, i);
    }
    else if(i < 100) {
      tempFilename.Format("%s.0%i.png", filename, i);
    } else {
      tempFilename.Format("%s.%i.png", filename, i);
    }
    _sprites[_spriteCounter] = new Sprite();
    _sprites[_spriteCounter]->LoadSprite((const char*)tempFilename);
    if(_sprites[_spriteCounter]->GetWidth() > _maxWidth) _maxWidth = _sprites[_spriteCounter]->GetWidth();
    if(_sprites[_spriteCounter]->GetHeight() > _maxHeight) _maxHeight = _sprites[_spriteCounter]->GetHeight();
    _spriteCounter++;
  }
  _numberOfFrames = frames;
  _animationSpeed = animationSpeed;
  _sequence = new AnimationSequence(sequence);
  SetCurrentAnimation(0);
}

void AnimatingSprite::SetCurrentAnimation(const String& animation) {
  _currentAnimation = _sequence->GetAnimation(animation)->_animationID;
}

void AnimatingSprite::SetCurrentAnimation(int index) {
  _currentAnimation = _sequence->GetAnimation(index)->_animationID;
}
