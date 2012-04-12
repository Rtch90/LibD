#include <SDL/SDL_mixer.h>

#include "Music.h"

ResourceManager<Music> musicManager;

Music::Music() : Resource() {
  _music = NULL;
}

Music::~Music() {
  if(_music) {
    Mix_FreeMusic(_music);
    _music = NULL;
  }
}

bool Music::Load(const std::string& filename) {
  _music = Mix_LoadMUS(filename.c_str());
  return _music != NULL;
}

void Music::Play(Music* music, int loops) {
  Stop();
  Mix_PlayMusic(music->_music, loops);
}

void Music::Stop() {
  Mix_HaltMusic();
}
