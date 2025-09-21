#pragma once

#include "graphics/Shader.h"
#include "models/IDrawable.h"
#include <vector>

class Sphere : public IDrawable {
  public:
    Sphere(unsigned int latitudeBands = 30, unsigned int longitudeBands = 30, float radius = 1.0f);
    ~Sphere() override;
    Sphere(const Sphere &) = delete;
    Sphere &operator=(const Sphere &) = delete;
    Sphere(Sphere &&) = delete;
    Sphere &operator=(Sphere &&) = delete;

    void draw(Shader &shader) const override;

  private:
    void generateVerticesAndIndices(unsigned int latitudeBands, unsigned int longitudeBands, float radius);
    void printVertexData() const;

    unsigned int VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};