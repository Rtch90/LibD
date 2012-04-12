#pragma once
#include "../Collision/AABB.h"

class NPC {
public:
  NPC(void);
  ~NPC(void);

  void Update(void);
  void Render(void);

private:
  AABB* _NPC;
};
