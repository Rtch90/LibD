#include <SDL/SDL_mixer.h>

#include "SoundEffect.h"

ResourceManager<SoundEffect> sfxManager;

SoundEffect::SoundEffect() {
  _chunk = NULL;
}

SoundEffect::~SoundEffect() {
  if(_chunk) {
    Mix_FreeChunk(_chunk);
    _chunk = NULL;
  }
}

bool SoundEffect::Load(const std::string& filename) {
  _chunk = Mix_LoadWAV(filename.c_str());
  return _chunk != NULL;
}

void SoundEffect::Play(SoundEffect* effect, int channel, int loops) {
  Mix_PlayChannel(channel, effect->_chunk, loops);
}

void SoundEffect::Stop(int channel) {
  Mix_HaltChannel(channel);
}

bool SoundEffect::IsPlaying(int channel) {
  return Mix_Playing(channel) == 1;
}