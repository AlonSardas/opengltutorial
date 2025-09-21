#pragma once

#include "IDrawable.h"
#include <vector>

class DrawableList : public IDrawable {
  public:
    std::vector<const IDrawable *> drawables;
    DrawableList() = default;

    void draw(Shader &shader) const override {
        for (const auto &drawable : drawables) {
            drawable->draw(shader);
        }
    }
};