/*
Following this chapter:
https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/3.1.2.shadow_mapping_base/shadow_mapping_base.cpp
*/

#include "ShadowRenderer.h"
#include <GLFW/glfw3.h>
#include <PlayerModel.h>
#include <models/primitives/Quad.h>

ShadowRenderer::ShadowRenderer(IControllable *const *agent, const Player *player, const PerspectiveProjection &proj)
    : currentAgent(agent), projection(proj), shader("shaders/lightWithShadow.vs", "shaders/lightWithShadow.fs"),
      pointLightSphereShader("shaders/transform.vs", "shaders/solidColor.fs") {
    playerModel = std::make_unique<PlayerModel>(player);
    models.push_back(playerModel.get());
}

ShadowRenderer::~ShadowRenderer() {}

void ShadowRenderer::init() {
    // TODO: Check some blinking issues

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // glDisable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glDisable(GL_CULL_FACE);

    dirLight.position = glm::vec3(-10.0f, 7.0f, -5.0f);
    dirLight.direction = normalize(glm::vec3(20.0f, -12.0f, 12.0f));
    dirLight.updateLightSpaceMatrix();
    dirLight.depthMap.resize(2048, 2048);
    pointLight.depthCubeMap.resize(1024, 1024);
    pointLight.farPlane = 250.0f;
    glClearColor(background.x, background.y, background.z, 1.0f);

    floorQuad = std::make_unique<Quad>(25.0f, 25.0f);
    // floorQuad = std::make_unique<Quad>();
    // floorTexture = std::make_unique<TexturedObject>(floorQuad.get(), "resources/wall.jpg", "diffuseTexture");
    floorTexture =
        std::make_unique<TexturedObject>(floorQuad.get(), "diffuseTexture", "resources/wood.png", GL_REPEAT, GL_REPEAT);
    floor = std::make_unique<SceneObject>(floorTexture.get());
    floor->transform.setRotation({glm::radians(-90.0f), 0.0f, 0.0f});
    floor->transform.setScale({120.0f, 120.0f, 1.0f});

    addCube2({-2.0f, 2.5f, -10.0}, glm::vec3(1.0f), {0.0f, 0.0f, 0.0f});
    addCube2({2.0f, 0.25f, 1.0}, {2.5f, 1.5f, 2.5f}, {0.0f, 0.0f, 0.0f});
    addCube2({-3.0f, 2.0f, 3.0}, {1.0f, 1.0f, 1.0f}, {glm::radians(60.0f), 0.0f, glm::radians(60.0f)});
    addCube2({10.0f, 2.0f, 3.0}, {1.0f, 1.0f, 1.0f}, {glm::radians(60.0f), 0.0f, glm::radians(60.0f)});
    addCube2({-3.0f, 2.0f, 13.0}, {1.0f, 1.0f, 1.0f}, {glm::radians(60.0f), 0.0f, glm::radians(60.0f)});

    addSphere({0.4f, 3.5f, -5.0f}, {2.5f, 0.5f, 1.5f}, {0.8f, 0.1f, 0.05f});
    addSphere({-1.4f, 2.5f, 10.0f}, {1.5f, 1.5f, 1.5f}, {0.1f, 0.3f, 0.6f});
    addSphere({-5.0f, 0.5f, -5.0f}, {1.5f, 1.5f, 1.5f}, {0.1f, 0.8f, 0.05f}, "resources/Pitz1.jpeg");

    models.push_back(floor.get());

    sphereData = std::make_unique<Sphere>(20, 20, 0.1f);
    sphereColor = std::make_unique<SolidColorObject>(sphereData.get());
    sphere = std::make_unique<SceneObject>(sphereColor.get());

    shader.use();
    shader.setInt("shadowMap", 1);
    shader.setInt("depthCubeMap", 2);
    shader.setFloat("farPlane", pointLight.farPlane);
}

void ShadowRenderer::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto camera = *currentAgent;
    const glm::mat4 &viewMatrix = camera->getViewMatrix();
    const glm::mat4 &projMatrix = projection.getMatrix();

    // Render depth of scene to texture
    glCullFace(GL_FRONT);

    dirLight.beginDepthPass(width, height);
    renderScene(dirLight.getShader());
    dirLight.endDepthPass();

    glm::vec3 lightPos(0.0f, 4.5f, 0.0f);
    lightPos.z = static_cast<float>(sin(glfwGetTime() * 0.5) * 3.0);
    sphere->transform.setPosition(lightPos);
    pointLight.position = lightPos;
    pointLight.beginDepthPass(width, height);
    renderScene(pointLight.getShader());
    pointLight.endDepthPass();

    glCullFace(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    shader.setVec3("viewPos", camera->getCameraPosition());
    shader.setVec3("lightDir", -dirLight.direction);
    shader.setMat4("lightSpaceMatrix", dirLight.getLightSpaceMatrix());
    shader.setVec3("lightPos", pointLight.position);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, dirLight.depthMap.getTexture());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, pointLight.depthCubeMap.getTexture());

    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projMatrix);
    // shader.setMat4("view", lightView);
    // shader.setMat4("projection", lightProjection);
    renderScene(shader);

    // Render the light source
    pointLightSphereShader.use();
    pointLightSphereShader.setMat4("view", viewMatrix);
    pointLightSphereShader.setMat4("projection", projMatrix);
    sphere->draw(pointLightSphereShader);
    // sphere->draw(shader);
}

void ShadowRenderer::onResize(int width, int height) {
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);
}

void ShadowRenderer::addCube(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation) {
    auto cubeData = std::make_unique<Cube>();
    auto texturedCube = std::make_unique<TexturedObject>(cubeData.get(), "diffuseTexture", "resources/container2.png");
    auto cube = std::make_unique<SceneObject>(texturedCube.get());
    cube->transform.setPosition(position);
    cube->transform.setScale(scale);
    cube->transform.setRotation(rotation);
    models.push_back(cube.get());
    allObjects.push_back(std::move(cubeData));
    allObjects.push_back(std::move(texturedCube));
    allObjects.push_back(std::move(cube));
}

void ShadowRenderer::addSphere(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &color,
                               const std::string &texturePath) {
    auto sphereData = std::make_unique<Sphere>();
    std::unique_ptr<TexturedObject> texturedSphere;
    if (texturePath.empty()) {
        texturedSphere = std::make_unique<TexturedObject>(sphereData.get(), "diffuseTexture", color.r * 255,
                                                          color.g * 255, color.b * 255, 255);
    } else {
        texturedSphere = std::make_unique<TexturedObject>(sphereData.get(), "diffuseTexture", texturePath);
    }
    auto sphere = std::make_unique<SceneObject>(texturedSphere.get());
    sphere->transform.setPosition(position);
    sphere->transform.setScale(scale);
    models.push_back(sphere.get());
    allObjects.push_back(std::move(sphereData));
    allObjects.push_back(std::move(texturedSphere));
    allObjects.push_back(std::move(sphere));
}

void ShadowRenderer::addCube2(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation) {
    // Here we use composition, so we can reuse the same cube data and texture for multiple cubes
    if (!cubaData) {
        // initialize once
        cubeList = std::make_unique<DrawableList>();
        cubaData = std::make_unique<Cube>();
        cubaData->setInnerDrawable(cubeList.get());
        defaultCubeDrawable = cubaData->getDefaultDrawable();
        texturedCube = std::make_unique<TexturedObject>(cubaData.get(), "diffuseTexture", "resources/container2.png");
        models.push_back(texturedCube.get());
    }

    auto cube = std::make_unique<SceneObject>(defaultCubeDrawable);
    cube->transform.setPosition(position);
    cube->transform.setScale(scale);
    cube->transform.setRotation(rotation);
    cubeList->drawables.push_back(cube.get());
    allObjects.push_back(std::move(cube));
}

void ShadowRenderer::renderScene(Shader &shader) {
    for (const auto &model : models) {
        model->draw(shader);
    }
}
