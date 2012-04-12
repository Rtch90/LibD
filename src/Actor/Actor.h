#pragma once

class Actor {
public:
  enum State {
    WALKING,
    RUNNING,
    ATTACKING,
  };

  enum Facing {
    FRONT,
    BACK,
    LEFT,
    RIGHT
  };

  Actor(void);
  ~Actor(void);

  float GetX(void)                      { return x; }
  float GetY(void)                      { return y; }
  float GetWidth(void)                  { return w; }
  float GetHeight(void)                 { return h; }

  void SetXY(float xArg, float yArg)    { x = xArg; y = yArg; }

private:
  const float VELOCITY;
  float x;
  float y;
  float w;
  float h;
};
