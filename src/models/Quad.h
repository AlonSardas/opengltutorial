#pragma once

#include "graphics/Shader.h"
#include <array>

class Quad {
  public:
    Quad();
    ~Quad();
    Quad(const Quad &) = delete;
    Quad &operator=(const Quad &) = delete;
    Quad(Quad &&) = delete;
    Quad &operator=(Quad &&) = delete;

    void draw(Shader &shader) const;

  private:
    unsigned int VAO, VBO;
    static const std::array<float, 30> vertices;
};