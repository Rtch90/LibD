#pragma once
#include <string>
#include "../Math/Vec2.h"
#include "../Math/Rect.h"

class Texture;

class Sprite {
public:
  Sprite();
  virtual ~Sprite();

  virtual void Update(float dt);
  virtual void Draw() const;
  virtual void DrawRegion(const Rect& src) const;

  virtual bool LoadSprite(const std::string& filename);

  const Vec2&     GetPosition() const     { return position; }
  float           GetX(void)              { return position.x; }
  float           GetY(void)              { return position.y; }
  const Vec2&     GetSize() const         { return size; }
  float           GetWidth()              { return size.x; }
  float           GetHeight()             { return size.y; }
  const Vec2&     GetScale() const        { return scale; }
  float           GetRotation() const     { return rotation; }
  Texture*        GetTexture()            { return texture; }
  const Texture*  GetTexture() const      { return texture; }

  void SetPosition(const Vec2& position)  { this->position = position; }
  void SetX(float xArg)                   { this->position.x = xArg; }
  void SetY(float yArg)                   { this->position.y = yArg; }
  void SetScale(const Vec2& scale)        { this->scale = scale; }
  void SetRotation(float rotation)        { this->rotation = rotation; }
  void SetTexture(Texture* texture);

protected:
  Vec2 position;
  Vec2 size;
  Vec2 scale;
  float rotation;
  Texture* texture;
};

