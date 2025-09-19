#pragma once

#include "graphics/Shader.h"

class IDrawable {
  public:
    virtual ~IDrawable() = default;
    virtual void draw(Shader &shader) const = 0;
};