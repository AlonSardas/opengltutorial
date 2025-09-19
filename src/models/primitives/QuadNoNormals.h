#pragma once

#include "graphics/Shader.h"
#include <array>

class QuadNoNormals {
  public:
    QuadNoNormals();
    ~QuadNoNormals();
    QuadNoNormals(const QuadNoNormals &) = delete;
    QuadNoNormals &operator=(const QuadNoNormals &) = delete;
    QuadNoNormals(QuadNoNormals &&) = delete;
    QuadNoNormals &operator=(QuadNoNormals &&) = delete;

    void draw(Shader &shader) const;

  private:
    unsigned int VAO, VBO;
    static const std::array<float, 30> vertices;
};