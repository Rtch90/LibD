#include "Sprite.h"
#include "../Texture/Texture.h"

Sprite::Sprite() {
  texture = NULL;
  size = Vec2(0.0f, 0.0f);
  scale = Vec2(1.0f, 1.0f);
  handle = Vec2(0.0f, 0.0f);
}

Sprite::~Sprite() {
}

void Sprite::Update(float dt) {
}

void Sprite::Draw() const {
  // Awesome artwork to describe this:
  // 0---------1
  // .         .
  // .         .
  // .         .
  // 3---------2

  Vec2 vertices[4] = {
    Vec2(handle.x, handle.y),
    Vec2(handle.x + size.x, handle.y),
    Vec2(handle.x + size.x, handle.y + size.y),
    Vec2(handle.x, handle.y + size.y),
  };

  Vec2 texCoords[4] = {
    Vec2(0.0f, 0.0f),
    Vec2(1.0f, 0.0f),
    Vec2(1.0f, 1.0f),
    Vec2(0.0f, 1.0f),
  };

  for(int i = 0; i < 4; i++){
    vertices[i].x *= scale.x;
    vertices[i].y *= scale.y;
  }

  glEnable(GL_TEXTURE_2D);
  BindTexture(texture->GetTexID());

  glBegin(GL_QUADS);
  glTexCoord2fv((const GLfloat*)&texCoords[0]);
  glVertex2fv((const GLfloat*)&vertices[0]);
  glTexCoord2fv((const GLfloat*)&texCoords[1]);
  glVertex2fv((const GLfloat*)&vertices[1]);
  glTexCoord2fv((const GLfloat*)&texCoords[2]);
  glVertex2fv((const GLfloat*)&vertices[2]);
  glTexCoord2fv((const GLfloat*)&texCoords[3]);
  glVertex2fv((const GLfloat*)&vertices[3]);
  glEnd();
}

void Sprite::SetTexture(Texture* texture) {
  this->texture = texture;
  this->size = Vec2(texture->GetWidth(), texture->GetHeight());
}
