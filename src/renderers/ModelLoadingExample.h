#pragma once

#include "cameras/QuatCamera.h"
#include "graphics/PerspectiveProjection.h"
#include "graphics/Shader.h"
#include "models/Cube.h"
#include "models/Model.h"
#include "renderers/IRenderer.h"
#include <optional>

class ModelLoadingExample : public IRenderer {
  public:
    ModelLoadingExample(const QuatCamera &c, PerspectiveProjection &proj);
    ~ModelLoadingExample() override;
    void init() override;
    void render() override;
    void onResize(int width, int height) override;

  private:
    const QuatCamera &camera;
    const PerspectiveProjection &projection;
    Shader shader;
    std::optional<Model> ourModel;
    std::optional<Cube> cube, cube2;

    static constexpr glm::vec3 background = glm::vec3(0.05f, 0.01f, 0.01f);
};