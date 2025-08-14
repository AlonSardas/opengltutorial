#pragma once

#include "PerspectiveProjection.h"
#include "Shader.h"
#include "cameras/QuatCamera.h"
#include "renderers/IRenderer.h"
#include "texture/Texture.h"
#include <array>

class Renderer : public IRenderer {
  public:
    Renderer(const QuatCamera &c, PerspectiveProjection &proj);
    ~Renderer() override;
    void init() override;
    void render() override;
    void onResize(int width, int height) override;

  private:
    const QuatCamera &camera;
    Shader shader;
    const PerspectiveProjection &projection;
    unsigned int VBO, VAO, EBO;
    glm::mat4 model;
    Texture texture1, texture2;

    static const std::array<glm::vec3, 10> &getCubePositions();
};