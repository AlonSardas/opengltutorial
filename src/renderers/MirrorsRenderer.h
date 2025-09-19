#pragma once

#include "cameras/QuatCamera.h"
#include "graphics/Framebuffer.h"
#include "graphics/PerspectiveProjection.h"
#include "graphics/Shader.h"
#include "models/LayeredMirror.h"
#include "models/Mirror.h"
#include "models/Model.h"
#include "models/SkyBox.h"
#include "models/primitives/Cube.h"
#include "models/primitives/ScreenQuad.h"
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
    void renderScene(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);
    void renderMirrorRecursive(std::vector<Mirror *> allMirrors, Mirror *mirror, int width, int height, int depth,
                               const glm::vec3 &viewerPos);
    void renderLayeredMirror(std::vector<LayeredMirror *> allMirrors, LayeredMirror *mirror, int width, int height,
                             int depth, int maxDepth, const glm::vec3 &viewerPos);

    float width = 0.0f, height = 0.0f;
    QuatCamera &camera;
    PerspectiveProjection &projection;
    Shader shader, screenShader;
    std::optional<Mirror> mirror1, mirror2;
    std::optional<LayeredMirror> layeredMirror1, layeredMirror2, layeredMirror3;
    std::optional<Model> scene;
    std::optional<SkyBox> skyBox;
    float scaleFactor = 0.05f;
    std::optional<Framebuffer> framebuffer;
    float rearViewWidth = 0.3f, rearViewHeight = 0.41f;

    std::optional<ScreenQuad> screenQuad;

    static constexpr glm::vec3 background = glm::vec3(0.9f, 0.9f, 0.9f);
};