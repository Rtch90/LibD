#pragma once
#include "../Sprite/Sprite.h"

class NPC {
public:
  NPC(void);
  ~NPC(void);

  void Update(void);
  void Render(void);

private:
  Sprite* _NPC;
};
