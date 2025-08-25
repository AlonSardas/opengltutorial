#pragma once

#include "graphics/Shader.h"

class ScreenQuad {
  public:
    ScreenQuad(float x, float y, float width, float height);
    ~ScreenQuad();
    ScreenQuad(const ScreenQuad &) = delete;
    ScreenQuad &operator=(const ScreenQuad &) = delete;
    ScreenQuad(ScreenQuad &&) = delete;
    ScreenQuad &operator=(ScreenQuad &&) = delete;
    void draw(Shader &shader);
    void setBounds(float x, float y, float width, float height);

  private:
    unsigned int VAO, VBO;
};