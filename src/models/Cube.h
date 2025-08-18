#pragma once

#include "Shader.h"
#include <array>

class Cube {
  public:
    Cube();
    ~Cube();
    void draw(Shader &shader);

  private:
    unsigned int VAO, VBO, EBO;
    const std::array<float, 108> vertices;
};