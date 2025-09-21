#pragma once

#include "graphics/Shader.h"
#include "models/IDrawable.h"
#include <array>

class Quad : public IDrawable {
  public:
    Quad(float texScaleU = 1.0f, float texScaleV = 1.0f);
    ~Quad();
    Quad(const Quad &) = delete;
    Quad &operator=(const Quad &) = delete;
    Quad(Quad &&) = delete;
    Quad &operator=(Quad &&) = delete;

    // void setTexture(const std::string &texturePath);
    void draw(Shader &shader) const override;

  private:
    unsigned int VAO, VBO;
    static const std::array<float, 48> vertices;
};