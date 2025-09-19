#pragma once

#include "cameras/QuatCamera.h"
#include "graphics/PerspectiveProjection.h"
#include "graphics/Shader.h"
#include "models/Model.h"
#include "models/primitives/Cube.h"
#include "renderers/IRenderer.h"
#include <optional>

class StencilGlowExample : public IRenderer {
  public:
    StencilGlowExample(const QuatCamera &c, PerspectiveProjection &proj);
    ~StencilGlowExample() override;
    void init() override;
    void render() override;
    void onResize(int width, int height) override;

  private:
    const QuatCamera &camera;
    const PerspectiveProjection &projection;
    Shader shader, textureShader;
    std::optional<Cube> cube1, cube2, cube3;

    Shader stencilShader;
    glm::vec3 color;

    static constexpr glm::vec3 background = glm::vec3(0.05f, 0.01f, 0.01f);
};