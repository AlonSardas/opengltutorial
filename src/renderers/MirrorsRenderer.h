#pragma once

#include "cameras/QuatCamera.h"
#include "graphics/Framebuffer.h"
#include "graphics/PerspectiveProjection.h"
#include "graphics/Shader.h"
#include "models/Cube.h"
#include "models/Mirror.h"
#include "models/Model.h"
#include "models/ScreenQuad.h"
#include "renderers/IRenderer.h"
#include <optional>

class MirrorsRenderer : public IRenderer {
  public:
    MirrorsRenderer(QuatCamera &c, PerspectiveProjection &proj);
    ~MirrorsRenderer() override;
    void init() override;
    void render() override;
    void onResize(int width, int height) override;

  private:
    void renderScene();

    float width = 0.0f, height = 0.0f;
    QuatCamera &camera;
    PerspectiveProjection &projection;
    Shader shader, screenShader;
    std::optional<Mirror> mirror1;
    std::optional<Model> scene;
    float scaleFactor = 0.05f;
    std::optional<Framebuffer> framebuffer;
    float rearViewWidth = 0.3f, rearViewHeight = 0.41f;

    std::optional<ScreenQuad> screenQuad;

    static constexpr glm::vec3 background = glm::vec3(0.9f, 0.9f, 0.9f);
};