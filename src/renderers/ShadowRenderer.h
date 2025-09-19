#pragma once

#include "IRenderer.h"
#include "Player.h"
#include "cameras/QuatCamera.h"
#include "graphics/DepthMap.h"
#include "graphics/PerspectiveProjection.h"
#include "graphics/Shader.h"
#include "models/IDrawable.h"
#include <memory>
#include <models/SceneObject.h>
#include <models/TexturedObject.h>
#include <vector>

class ShadowRenderer : public IRenderer {
  public:
    ShadowRenderer(IControllable *const *agent, const Player *player, const PerspectiveProjection &proj);
    ~ShadowRenderer() override;
    void init() override;
    void render() override;
    void onResize(int width, int height) override;

  private:
    int width = 0, height = 0;
    IControllable *const *currentAgent;
    const PerspectiveProjection &projection;
    Shader shader, depthShader;
    DepthMap depthMap;
    std::vector<IDrawable *> models;
    std::vector<std::unique_ptr<IDrawable>> allObjects;
    static constexpr glm::vec3 background = glm::vec3(0.1f, 0.1f, 0.01f);

    std::unique_ptr<IDrawable> playerModel, floorQuad;
    std::unique_ptr<TexturedObject> floorTexture;
    std::unique_ptr<SceneObject> floor;

    void addCube(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation);
    void renderScene(Shader &shader);
};