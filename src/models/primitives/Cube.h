#pragma once

#include "ArrayDrawable.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "models/IDrawable.h"
#include <array>
#include <memory>
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
    ArrayDrawable *getDefaultDrawable() const { return defaultDrawable.get(); }
    void setInnerDrawable(IDrawable *d) { inner = d; }

  private:
    unsigned int VAO, VBO;
    std::optional<Texture> texture;
    static const std::array<float, 288> vertices;
    std::unique_ptr<ArrayDrawable> defaultDrawable;
    IDrawable *inner;
};