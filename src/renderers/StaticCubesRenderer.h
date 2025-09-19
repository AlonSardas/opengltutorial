#pragma once

#include "cameras/QuatCamera.h"
#include "graphics/PerspectiveProjection.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "renderers/IRenderer.h"
#include <array>

class StaticCubesRenderer : public IRenderer {
  public:
    StaticCubesRenderer(const QuatCamera &c, PerspectiveProjection &proj);
    ~StaticCubesRenderer() override;
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