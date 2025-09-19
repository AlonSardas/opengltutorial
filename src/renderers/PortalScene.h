#pragma once

#include "IControllable.h"
#include "Player.h"
#include "PlayerModel.h"
#include "graphics/PerspectiveProjection.h"
#include "models/Model.h"
#include "models/Portal.h"
#include "models/SkyBox.h"
#include "renderers/IRenderer.h"
#include <array>
#include <optional>

class PortalScene : public IRenderer {
  public:
    PortalScene(IControllable *const *currentAgent, PerspectiveProjection &proj, Player *player, POVPlayer *povPlayer);
    ~PortalScene() override;
    void init() override;
    void render() override;
    void onResize(int width, int height) override;

  private:
    IControllable *const *currentAgent;
    const PerspectiveProjection &projection;

    std::optional<PlayerModel> playerModel;
    std::optional<Model> obj1;
    std::optional<Cube> obj2, obj3;
    std::optional<SkyBox> skyBox;
    std::optional<Portal> portal1, portal2;
    std::optional<QuadNoNormals> floor;
    std::optional<Texture> floorTexture;

    Shader shader, floorShader;

    void recursiveDraw(Portal *portal, const glm::mat4 &viewMat, const glm::mat4 &projMat, size_t maxRecursionLevel,
                       size_t recursionLevel);
    void nonRecursiveDraw();
    void drawRestOfScene(const glm::mat4 &viewMat, const glm::mat4 &projMat);
    void drawRestOfScene();

    static constexpr glm::vec3 background = glm::vec3(0.1f, 0.9f, 0.01f);
};