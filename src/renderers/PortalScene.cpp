/*
Nice video:
https://www.youtube.com/watch?v=cWpFZbjtSQg
*/

#include "PortalScene.h"
#include "stb/stb_image.h"
#include <glad/glad.h>

// TODO: Add teleportation

void printStencilBuffer(int width, int height);

PortalScene::PortalScene(const QuatCamera &c, PerspectiveProjection &proj)
    : camera(c), projection(proj), shader("shaders/vertexTransformer2.vs", "shaders/fragmentAdapterWithTexture.fs"),
      floorShader("shaders/vertexTransformer.vs", "shaders/fragmentAdapterWithTexture.fs") {}

PortalScene::~PortalScene() {}

void PortalScene::init() {
    portal1.emplace("portal1", glm::vec3(2.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), 0, 6.0f, 4.0f);
    // portal2.emplace("portal2", glm::vec3(-5.0f, 2.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0, 6.0f, 4.0f);
    portal2.emplace("portal2", glm::vec3(-5.0f, 1.0f, -0.0f), glm::vec3(1.0f, 0.0f, 0.1f), 0, 6.0f, 4.0f);

    portal1->setDestination(&portal2.value());
    portal2->setDestination(&portal1.value());

    stbi_set_flip_vertically_on_load(true);
    // obj1.emplace("../models/backpack/backpack.obj");
    // obj1.emplace("../models/RockingHorse/model.dae");
    // obj1.emplace("../models/bicolor-cat/source/bicolor Cat.glb");
    obj1.emplace("../models/the-cats-body/model.obj");

    obj2.emplace();
    obj2->setTexture("resources/Pitz1.jpeg");

    obj3.emplace();
    obj3->setTexture("resources/container2.png");

    floor.emplace();

    floorTexture.emplace();
    floorTexture->bind(0);
    floorTexture->setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    floorTexture->setMinMagFilters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    floorTexture->loadImage("resources/wall.jpg", true);

    stbi_set_flip_vertically_on_load(false);
    skyBox.emplace("resources/skybox");
}

void PortalScene::render() {
    glStencilMask(0xFF);
    glClearColor(background.x, background.y, background.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // glEnable(GL_DEPTH_TEST);
    // drawRestOfScene();
    // portal1->drawBlank(camera.getViewMatrix(), projection.getMatrix());
    // portal2->drawBlank(camera.getViewMatrix(), projection.getMatrix());
    // return;

    // nonRecursiveDraw();

    std::cout << "Render start" << std::endl;
    recursiveDraw(&portal2.value(), camera.getViewMatrix(), projection.getMatrix(), 6, 0);
    recursiveDraw(&portal1.value(), camera.getViewMatrix(), projection.getMatrix(), 3, 0);

    // portal1->drawBlank(camera.getViewMatrix(), projection.getMatrix());

    // Draw the portal just to depth buffer so it hides the things behind it
    // glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    // glDisable(GL_STENCIL_TEST);
    // glEnable(GL_DEPTH_TEST);
    // glClear(GL_DEPTH_BUFFER_BIT);
    // portal1->drawBlank(camera.getViewMatrix(), projection.getMatrix());
    // portal1->draw(camera.getViewMatrix(), projection.getMatrix());

    // Draw rest of the scene
    glDisable(GL_STENCIL_TEST);
    // glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    drawRestOfScene();
    // glDepthFunc(GL_LEQUAL);
    // portal1->draw(camera.getViewMatrix(), projection.getMatrix());
    // portal2->draw(camera.getViewMatrix(), projection.getMatrix());
}

void PortalScene::onResize(int width, int height) { glViewport(0, 0, width, height); }

void PortalScene::recursiveDraw(Portal *portal, const glm::mat4 &viewMat, const glm::mat4 &projMat,
                                size_t maxRecursionLevel, size_t recursionLevel) {
    // We redraw the portal frame and each time the stencil buffer is increased by one

    auto nextViewMatrix = portal->getViewMat(viewMat);
    std::optional<glm::mat4> nextProjMatrix = std::nullopt;
    if (nextViewMatrix) {
        nextProjMatrix = portal->getDestination()->getProjMat(nextViewMatrix.value(), projMat);
    }

    if (recursionLevel == maxRecursionLevel) {
        std::cout << "reached max depth for " << portal->getName() << std::endl;
    }
    if (recursionLevel == maxRecursionLevel || !nextViewMatrix || !nextProjMatrix) {
        // Re-enable color and depth writing
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);

        glStencilFunc(GL_LEQUAL, recursionLevel, 0xFF);
        portal->drawBlank(viewMat, projMat);
        portal->draw(viewMat, projMat);
        return;
    }
    if (recursionLevel == 0) {
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glStencilMask(0xFF);
        glClear(GL_STENCIL_BUFFER_BIT);
        // glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Enable stencil writing
        glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

        // Disable writing to color and depth buffers
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);
    }

    // Draw this portal position to stencil
    glStencilFunc(GL_LEQUAL, recursionLevel, 0xFF);
    portal->drawBlank(viewMat, projMat);

    recursiveDraw(portal, nextViewMatrix.value(), nextProjMatrix.value(), maxRecursionLevel, recursionLevel + 1);
    glStencilFunc(GL_LEQUAL, recursionLevel + 1, 0xFF);
    drawRestOfScene(nextViewMatrix.value(), nextProjMatrix.value());

    // Mark portal in depth buffer
    glDepthFunc(GL_ALWAYS); // Always pass depth test
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    portal->drawBlank(viewMat, projMat);             // Stamp portal depth back
    glDepthFunc(GL_LESS);                            // Restore default
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Restore color writes

    glStencilFunc(GL_LEQUAL, recursionLevel, 0xFF);
    portal->draw(viewMat, projMat);
    // skyBox->draw(viewMat, projMat);
}

void PortalScene::nonRecursiveDraw() {
    // First put 1s where portals should be. Note that we keep depth test to account for portals being hidden (behind
    // other objects)
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);

    // Enable stencil writing
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);      // always pass
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR); // increment where depth passes

    // Disable writing to color and depth buffers
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    portal1->drawBlank(camera.getViewMatrix(), projection.getMatrix());
    // portal2->drawBlank(camera.getViewMatrix(), projection.getMatrix());

    // printStencilBuffer(80, 800);

    glStencilMask(0x00);
    // Re-enable color and depth writing
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    // // Only draw where stencil >= 1
    glStencilFunc(GL_LEQUAL, 1, 0xFF);
    drawRestOfScene(portal1->getViewMat(camera.getViewMatrix()).value(),
                    portal1->getProjMat(camera.getViewMatrix(), projection.getMatrix()).value());

    // Now portal 2
    glClear(GL_STENCIL_BUFFER_BIT);
    glEnable(GL_STENCIL_TEST);
    // glEnable(GL_DEPTH_TEST);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

    // Disable writing to color and depth buffers
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    portal2->drawBlank(camera.getViewMatrix(), projection.getMatrix());
    glStencilMask(0x00);
    // Re-enable color and depth writing
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    // Only draw where stencil >= 1
    glStencilFunc(GL_LEQUAL, 1, 0xFF);
    // TODO: This will result in null ptr errors
    drawRestOfScene(portal2->getViewMat(camera.getViewMatrix()).value(),
                    portal2->getProjMat(camera.getViewMatrix(), projection.getMatrix()).value());

    // Here we only update depth buffer with the portal position
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    portal1->drawBlank(camera.getViewMatrix(), projection.getMatrix());
    portal2->drawBlank(camera.getViewMatrix(), projection.getMatrix());
}

void PortalScene::drawRestOfScene(const glm::mat4 &viewMat, const glm::mat4 &projMat) {
    shader.use();
    shader.setMat4("view", viewMat);
    shader.setMat4("projection", projMat);

    glm::mat4 model = glm::mat4(1.0f);
    float scale = 1.0f;
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    shader.setMat4("model", model);
    obj1->draw(shader);

    model = glm::mat4(1.0f);
    scale = 1.0f;
    model = glm::translate(model, glm::vec3(3.0f, 1.0f, 0.2f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    shader.setMat4("model", model);
    obj2->draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.2f, 1.0f, 4.2f));
    model = glm::scale(model, glm::vec3(4.0f, 1.0f, 1.0f));
    shader.setMat4("model", model);
    obj3->draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(12.0f, 12.0f, 1.0f));
    floorShader.use();
    floorShader.setMat4("view", viewMat);
    floorShader.setMat4("projection", projMat);
    floorShader.setMat4("model", model);
    floorTexture->bind(0);
    shader.setInt("texture_diffuse1", 0);
    floor->draw(floorShader);

    skyBox->draw(viewMat, projMat);
}

void PortalScene::drawRestOfScene() {
    drawRestOfScene(camera.getViewMatrix(), projection.getMatrix());
    // portal1->draw(camera.getViewMatrix(), projection.getMatrix());
    // portal2->draw(camera.getViewMatrix(), projection.getMatrix());
}

void printStencilBuffer(int width, int height) {
    std::cout << "--------------------- Stencil buffer ----------------------" << std::endl;
    std::vector<GLubyte> stencilData(width * height);
    glReadPixels(0, 0, width, height, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, stencilData.data());

    // Print or save to file
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::cout << (int)stencilData[y * width + x] << "";
            if ((int)stencilData[y * width + x] == 1) {
                std::cout << "got 1!!!!!!!!!!!!!!!!!!!!" << std::endl;
                return;
            }
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}