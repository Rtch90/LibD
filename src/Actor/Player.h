#pragma once
#include "../Actor/Actor.h"
#include "../Sprite/Sprite.h"
#include "../Global/Globals.h"
#include "../System/Debug.h"
#include "../IO/Input.h"

class Player : public Actor{
public:
  Player(void);
  ~Player(void);

  void Update(float dt);
  void Render(void);

private:
  void Move(float dt);
};
