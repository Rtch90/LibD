#pragma once

#include "../Math/Vec2.h"

class Texture;

class Sprite {
public:
  Sprite();
  virtual ~Sprite();

  virtual void Update(float dt);
  virtual void Draw() const;

  const Vec2&     GetHandle() const { return handle; }
  const Vec2&     GetSize() const { return size; }
  const Vec2&     GetScale() const { return scale; }
  float           GetRotation() const { return rotation; }
  Texture*        GetTexture() { return texture; }
  const Texture*  GetTexture() const { return texture; }

  void SetHandle(const Vec2& handle) { this->handle = handle; }
  void SetScale(const Vec2& scale) { this->scale = scale; }
  void SetRotation(float rotation) { this->rotation = rotation; }
  void SetTexture(Texture* texture);

protected:
  Vec2 handle;
  Vec2 size;
  Vec2 scale;
  float rotation;
  Texture* texture;
};

