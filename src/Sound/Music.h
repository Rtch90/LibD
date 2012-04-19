#pragma once

#include "../System/ResourceManager.h"

struct _Mix_Music;

class Music : public Resource {
  template<class T> friend class ResourceManager;

public:
  Music();
  ~Music();

  bool Load(const std::string& filename);

  static void Play(Music* music, int loops);
  static void Stop();
  static void SetVolume(int volume);

private:
  struct _Mix_Music* _music;
};

extern ResourceManager<Music> musicManager;
