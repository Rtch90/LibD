#pragma once
#include "../Sprite/Sprite.h"
#include "../Actor/Actor.h"

class NPC : public Actor {
public:
  NPC(Level* level);
  ~NPC(void);

  void Update(float dt);
  void Render(void);

private:
  void Move(float dt);
};
