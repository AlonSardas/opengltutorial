#pragma once

#include "Shader.h"
#include "texture/Texture.h"
#include <array>
#include <optional>

class Cube {
  public:
    Cube();
    ~Cube();
    Cube(const Cube &) = delete;
    Cube &operator=(const Cube &) = delete;
    Cube(Cube &&) = delete;
    Cube &operator=(Cube &&) = delete;

    void setWithNormals(bool other) { withNormals = other; }
    void setTexture(const std::string &texturePath);
    void draw(Shader &shader);

  private:
    unsigned int VAO, VBO;
    bool withNormals = false;
    std::optional<Texture> texture;
    static const std::array<float, 288> vertices;
};