#pragma once

#include "PerspectiveProjection.h"
#include "QuatCamera.h"
#include "Shader.h"
#include "renderers/IRenderer.h"
#include <array>

class LightingRenderer : public IRenderer {
  public:
    LightingRenderer(const QuatCamera &c, PerspectiveProjection &proj);
    ~LightingRenderer() override;
    void init() override;
    void render() override;
    void onResize(int width, int height) override;

  private:
    const QuatCamera &camera;
    const PerspectiveProjection &projection;
    Shader lightingShader, lightCubeShader;

    unsigned int lightVAO, VAO, VBO;
    glm::vec3 lightPos;
};