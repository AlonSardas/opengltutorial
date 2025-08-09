#pragma once

#include "Shader.h"
#include "Texture.h"
#include <array>
#include <glm/glm.hpp>

class Renderer {
  public:
    Renderer();
    ~Renderer();
    void init();
    void render();
    void onResize(int width, int height);

  private:
    Shader shader;
    unsigned int VBO, VAO, EBO;
    glm::mat4 model, view, projection;
    Texture texture1, texture2;
    static const std::array<glm::vec3, 10> &getCubePositions();
    static const std::array<float, 180> &getCubeVertices();
};