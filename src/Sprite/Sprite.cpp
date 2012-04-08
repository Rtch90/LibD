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

  Vec2 scaledSize(size.x*scale.x, size.y*scale.y);

  Vec2 vertices[4] = {
    Vec2(0.0f, 0.0f),
    Vec2(scaledSize.x, 0.0f),
    Vec2(scaledSize.x, scaledSize.y),
    Vec2(0.0f, scaledSize.y),
  };

  Vec2 texCoords[4] = {
    Vec2(0.0f, 0.0f),
    Vec2(1.0f, 0.0f),
    Vec2(1.0f, 1.0f),
    Vec2(0.0f, 1.0f),
  };

  glEnable(GL_TEXTURE_2D);
  BindTexture(texture->GetTexID());

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // Temporary solution.
  Vec2 halfScaledSize = scaledSize / 2.0f;
  glTranslatef(handle.x + halfScaledSize.x, handle.y + halfScaledSize.y, 0.0f);
  glRotatef(rotation, 0.0f, 0.0f, 1.0f);
  glTranslatef(-halfScaledSize.x, -halfScaledSize.y, 0.0f);

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

  glPopMatrix();
}

void Sprite::SetTexture(Texture* texture) {
  this->texture = texture;
  this->size = Vec2((float)texture->GetWidth(), (float)texture->GetHeight());
}
