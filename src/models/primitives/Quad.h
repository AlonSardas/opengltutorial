#pragma once

#include "graphics/Shader.h"
#include "models/IDrawable.h"
#include <array>

class Quad : public IDrawable {
  public:
    Quad();
    ~Quad();
    Quad(const Quad &) = delete;
    Quad &operator=(const Quad &) = delete;
    Quad(Quad &&) = delete;
    Quad &operator=(Quad &&) = delete;

    // void setTexture(const std::string &texturePath);
    void draw(Shader &shader) const;

  private:
    unsigned int VAO, VBO;
    static const std::array<float, 48> vertices;
};