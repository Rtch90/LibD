#include "../Texture/Texture.h"

// We will derive from an Actor class at some point.
class Player {
  Player(void);
  ~Player(void);
  
  void Prepare(void);
  void Render(void);
  void ProcessEvents(void);  
};
