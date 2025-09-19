#pragma once

#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "models/IDrawable.h"
#include <array>
#include <optional>

class Cube : public IDrawable {
  public:
    Cube();
    ~Cube() override;
    Cube(const Cube &) = delete;
    Cube &operator=(const Cube &) = delete;
    Cube(Cube &&) = delete;
    Cube &operator=(Cube &&) = delete;

    void setTexture(const std::string &texturePath);
    void draw(Shader &shader) const override;

  private:
    unsigned int VAO, VBO;
    std::optional<Texture> texture;
    static const std::array<float, 288> vertices;
};