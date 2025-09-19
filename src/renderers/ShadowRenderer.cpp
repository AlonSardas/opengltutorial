/*
Following this chapter:
https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/3.1.2.shadow_mapping_base/shadow_mapping_base.cpp
*/

#include "ShadowRenderer.h"
#include <PlayerModel.h>
#include <models/primitives/Quad.h>

ShadowRenderer::ShadowRenderer(IControllable *const *agent, const Player *player, const PerspectiveProjection &proj)
    : currentAgent(agent), projection(proj), shader("shaders/lightWithShadow.vs", "shaders/lightWithShadow.fs"),
      depthShader("shaders/depthShader.vs", "shaders/emptyFragment.fs") {
    depthShader.setUsesTexture(false);
    playerModel = std::make_unique<PlayerModel>(player);
    models.push_back(playerModel.get());
}

ShadowRenderer::~ShadowRenderer() {}

void ShadowRenderer::init() {
    glEnable(GL_DEPTH_TEST);

    depthMap.resize(1024, 1024);
    glClearColor(background.x, background.y, background.z, 1.0f);

    floorQuad = std::make_unique<Quad>();
    floorTexture = std::make_unique<TexturedObject>(floorQuad.get(), "resources/wall.jpg", "diffuseTexture");
    floor = std::make_unique<SceneObject>(floorTexture.get());
    floor->transform.setRotation({glm::radians(-90.0f), 0.0f, 0.0f});
    floor->transform.setScale({12.0f, 12.0f, 1.0f});

    addCube({0.0f, 2.5f, 0.0}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f});
    addCube({2.0f, 0.25f, 1.0}, {1.5f, 0.5f, 1.5f}, {0.0f, 0.0f, 0.0f});
    addCube({-3.0f, 2.0f, 3.0}, {1.0f, 1.0f, 1.0f}, {glm::radians(60.0f), 0.0f, glm::radians(60.0f)});
    models.push_back(floor.get());

    shader.use();
    shader.setInt("shadowMap", 1);
}

void ShadowRenderer::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto camera = *currentAgent;
    const glm::mat4 &viewMatrix = camera->getViewMatrix();
    const glm::mat4 &projMatrix = projection.getMatrix();

    glm::vec3 lightPos(-4.0f, 5.0f, -1.0f);
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 0.1f, far_plane = 100.0f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
    // render scene from light's point of view
    depthShader.use();
    depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    depthMap.bind(0, 0, width, height);
    depthMap.clear();
    renderScene(depthShader);
    depthMap.unbind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    shader.setVec3("viewPos", camera->getPosition());
    shader.setVec3("lightPos", lightPos);
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap.getTexture());

    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projMatrix);

    renderScene(shader);
}

void ShadowRenderer::onResize(int width, int height) {
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);
}

void ShadowRenderer::addCube(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation) {
    auto cubeData = std::make_unique<Cube>();
    auto texturedCube = std::make_unique<TexturedObject>(cubeData.get(), "resources/container2.png", "diffuseTexture");
    auto cube = std::make_unique<SceneObject>(texturedCube.get());
    cube->transform.setPosition(position);
    cube->transform.setScale(scale);
    cube->transform.setRotation(rotation);
    models.push_back(cube.get());
    allObjects.push_back(std::move(cubeData));
    allObjects.push_back(std::move(texturedCube));
    allObjects.push_back(std::move(cube));
}

void ShadowRenderer::renderScene(Shader &shader) {
    for (const auto &model : models) {
        model->draw(shader);
    }
}
