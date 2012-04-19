#pragma once

#include "../System/ResourceManager.h"

struct Mix_Chunk;

class SoundEffect : public Resource {
  template<class T> friend class ResourceManager;

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

extern ResourceManager<SoundEffect> sfxManager;
