#pragma once

#include "../System/ResourceManager.h"

struct Mix_Chunk;

class SoundEffect : public saracraft::util::Resource {
  template<class T> friend class saracraft::util::ResourceManager;

public:
  SoundEffect();
  ~SoundEffect();

  bool Load(const std::string& filename);

  static void Play(SoundEffect* effect, int channel, int loops);
  static void Stop(int channel);
  static bool IsPlaying(int channel);
  static void SetVolume(int channel, int volume);

private:
  Mix_Chunk* _chunk;
};

extern saracraft::util::ResourceManager<SoundEffect> sfxManager;
