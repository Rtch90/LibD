#include "Sprite.h"
#include "../Texture/Texture.h"

Sprite::Sprite() {
  texture = NULL;
  size = Vec2(0.0f, 0.0f);
  scale = Vec2(1.0f, 1.0f);
  position = Vec2(0.0f, 0.0f);
  rotation = 0.0f;
}

Sprite::~Sprite() {
  if(texture) {
    textureManager.Destroy(texture);
    texture = NULL;
  }
}

void Sprite::Update(float dt) {
}

void Sprite::Draw() const {
  DrawRegion(Rect(0.0f, 0.0f, (float)texture->GetWidth(), (float)texture->GetHeight()));
}

void Sprite::Draw(float x, float y) {
  SetPosition(Vec2(x, y));
  DrawRegion(Rect(0.0f, 0.0f, (float)texture->GetWidth(), (float)texture->GetHeight()));
}

void Sprite::DrawRegion(const Rect& src) const {
  const float uvX = src.x / (float)texture->GetWidth();
  const float uvY = src.y / (float)texture->GetHeight();
  const float uvW = src.w / (float)texture->GetWidth();
  const float uvH = src.h / (float)texture->GetHeight();

  // Awesome artwork to describe this:
  // 0---------1
  // .         .
  // .         .
  // .         .
  // 3---------2

  Vec2 scaledSize(src.w*scale.x, src.h*scale.y);

  Vec2 vertices[4] = {
    Vec2(0.0f, 0.0f),
    Vec2(scaledSize.x, 0.0f),
    Vec2(scaledSize.x, scaledSize.y),
    Vec2(0.0f, scaledSize.y),
  };

  Vec2 texCoords[4] = {
    Vec2(uvX, uvY),
    Vec2(uvX + uvW, uvY),
    Vec2(uvX + uvW, uvY + uvH),
    Vec2(uvX, uvY + uvH),
  };

  glEnable(GL_TEXTURE_2D);
  BindTexture(texture->GetTexID());

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // Temporary solution.
  Vec2 halfScaledSize = scaledSize / 2.0f;
  glTranslatef(position.x + halfScaledSize.x, position.y + halfScaledSize.y, 0.0f);
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

bool Sprite::LoadSprite(const std::string& filename) {
  Texture* newTexture = textureManager.Load(filename);
  if(newTexture) {
    if(texture) {
      textureManager.Destroy(texture);
    }
    size.x = (float)newTexture->GetWidth();
    size.y = (float)newTexture->GetHeight();
    texture = newTexture;
    return true;
  }
  return false;
}

void Sprite::SetTexture(Texture* texture) {
  this->texture = texture;
  this->size = Vec2((float)texture->GetWidth(), (float)texture->GetHeight());
}
