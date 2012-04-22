#pragma once
#include "../Actor/Actor.h"
#include "../Sprite/Sprite.h"
#include "../Global/Globals.h"
#include "../System/Debug.h"
#include "../IO/Input.h"

class Level;

class Player : public Actor{
public:
  Player(Level* level);
  ~Player(void);

  void Update(float dt);
  void Render(void);

private:
  void Move(float dt);
};
