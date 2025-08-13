#pragma once

#include "PerspectiveProjection.h"
#include "Shader.h"
#include "cameras/QuatCamera.h"
#include "renderers/IRenderer.h"
#include "texture/Texture.h"
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
    Texture texture, textureSpecular;

    unsigned int lightVAO, VAO, VBO;
    glm::vec3 lightPos;

    static constexpr glm::vec3 background = glm::vec3(0.05f, 0.01f, 0.01f);

    static const std::array<glm::vec3, 10> &getCubePositions();
};