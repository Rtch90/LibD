#pragma once

#include "../System/ResourceManager.h"

struct _Mix_Music;

class Music : public saracraft::util::Resource {
  template<class T> friend class saracraft::util::ResourceManager;

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

extern saracraft::util::ResourceManager<Music> musicManager;
