#pragma once

#include "IDrawable.h"
#include "Transform.h"

class SceneObject : public IDrawable {
  public:
    SceneObject(const IDrawable *drawable) : drawable(drawable) {}

    void draw(Shader &shader) const override {
        shader.setMat4("model", transform.getModelMatrix());
        drawable->draw(shader);
    }

    Transform transform;

  private:
    const IDrawable *drawable;
};