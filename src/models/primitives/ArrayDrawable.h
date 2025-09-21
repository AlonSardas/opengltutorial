#pragma once

#include "models/IDrawable.h"
#include <glad/glad.h>

class ArrayDrawable : public IDrawable {
  private:
    GLenum mode;
    GLint first;
    GLsizei count;

  public:
    ArrayDrawable(GLenum mode, GLint first, GLsizei count) : mode(mode), first(first), count(count) {}

    void draw(Shader &shader) const override { glDrawArrays(mode, first, count); }
};