#pragma once

#include "IRenderer.h"
#include "Player.h"
#include "cameras/QuatCamera.h"
#include "graphics/PerspectiveProjection.h"
#include "graphics/Shader.h"
#include "lights/DirectionalLight.h"
#include "lights/PointLight.h"
#include "models/DrawableList.h"
#include "models/IDrawable.h"
#include "models/SceneObject.h"
#include "models/SolidColorObject.h"
#include "models/TexturedObject.h"
#include "models/primitives/Cube.h"
#include "models/primitives/Sphere.h"
#include <memory>
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
    Shader shader;
    DirectionalLight dirLight;
    PointLight pointLight;
    std::vector<IDrawable *> models;
    std::vector<std::unique_ptr<IDrawable>> allObjects;
    static constexpr glm::vec3 background = glm::vec3(0.1f, 0.1f, 0.01f);

    std::unique_ptr<IDrawable> playerModel, floorQuad;
    std::unique_ptr<TexturedObject> floorTexture;
    std::unique_ptr<SceneObject> floor;
    std::unique_ptr<DrawableList> cubeList;
    std::unique_ptr<Cube> cubaData;
    std::unique_ptr<TexturedObject> texturedCube;
    std::unique_ptr<Sphere> sphereData;
    std::unique_ptr<SolidColorObject> sphereColor;
    std::unique_ptr<SceneObject> sphere;
    Shader pointLightSphereShader;
    IDrawable *defaultCubeDrawable = nullptr;

    void addCube(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation);
    void addSphere(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3& color, const std::string& texturePath = "");
    void addCube2(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation);
    void renderScene(Shader &shader);
};