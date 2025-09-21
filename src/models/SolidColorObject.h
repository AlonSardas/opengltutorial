#pragma once

#include "IDrawable.h"

class SolidColorObject : public IDrawable {
  public:
    SolidColorObject(const IDrawable *drawable) : drawable(drawable) {}

    void draw(Shader &shader) const override {
        shader.setVec3("color", color);
        drawable->draw(shader);
    }

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

  private:
    const IDrawable *drawable;
};