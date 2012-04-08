#include "../Texture/Texture.h"
#include "../Sprite/Sprite.h"
#include "../Global/Globals.h"
#include "../System/Debug.h"
#include "../IO/Input.h"

class Sprite;

// We will derive from an Actor class at some point.
class Player {
public:
  Player(void);
  ~Player(void);

  void Prepare(void);
  void Render(void);
  void ProcessEvents(void);

private:
  Sprite*   _player;
  Texture*  _playerTexture;
  float     _rotationAngle;
};
