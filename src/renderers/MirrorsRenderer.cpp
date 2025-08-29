/*
Based on
https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/5.2.framebuffers_exercise1/framebuffers_exercise1.cpp
*/

#include "MirrorsRenderer.h"

MirrorsRenderer::MirrorsRenderer(QuatCamera &c, PerspectiveProjection &proj)
    : camera(c), projection(proj), shader("shaders/vertexTransformer2.vs", "shaders/fragmentAdapterWithTexture.fs"),
      screenShader("shaders/framebufferScreen.vs", "shaders/framebufferScreen.fs") {}

MirrorsRenderer::~MirrorsRenderer() {}

void MirrorsRenderer::onResize(int width, int height) {
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);
    framebuffer->resize(width * rearViewWidth, height * rearViewHeight);
    // std::cout << (float)framebuffer->getWidth() / (float)framebuffer->getHeight() << "asdasd" << std::endl;
}

void MirrorsRenderer::init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // scene.emplace("../models/backpack/backpack.obj");
    framebuffer.emplace();
    screenQuad.emplace(-rearViewWidth, 1 - rearViewHeight * 2, rearViewWidth * 2, rearViewHeight * 2);
    mirror1.emplace(glm::vec3(20.0f, 4.0f, -30.0f), glm::vec3(1.0f, 0.0f, -0.0f), 4, 6, 900, 900);
    mirror2.emplace(glm::vec3(10.0f, 4.5f, -30.0f), glm::vec3(-1.0f, 0.0f, -0.1f), 4, 6, 900, 900);
    layeredMirror1.emplace(glm::vec3(15.0f, 4.5f, -33.0f), glm::vec3(-0.0f, 0.0f, -1.0f), 7, 6, 1000, 1000, 15);
    layeredMirror2.emplace(glm::vec3(19.0f, 4.9f, -27.0f), glm::vec3(-0.0f, 0.01f, 1.0f), 4, 6, 1000, 1000, 15);
    layeredMirror3.emplace(glm::vec3(11.0f, 4.9f, -26.0f), glm::vec3(-0.0f, 0.01f, 1.0f), 3, 6, 1000, 1000, 15);

    camera.moveForward(25);
    camera.moveRight(10);

    skyBox.emplace("resources/skybox");

    // stbi_set_flip_vertically_on_load(true);
    scene.emplace("../models/Apocalyptic City/Apocalyptic City.obj");

    projection.setClippingPlanes(0.1f, 1000.0f);
}

void MirrorsRenderer::render() {
    bool basicReflection = false;

    if (basicReflection) {
        mirror2->update(camera.getPosition());

        // Inner reflection
        mirror1->update(mirror2->getReflectedPos());
        mirror1->beginMirrorRender(width, height);
        renderScene(mirror1->getViewMatrix(), mirror1->getProjectionMatrix());
        mirror1->endMirrorRender();

        mirror2->beginMirrorRender(width, height);
        renderScene(mirror2->getViewMatrix(), mirror2->getProjectionMatrix());
        mirror1->draw(mirror2->getViewMatrix(), mirror2->getProjectionMatrix());
        mirror2->endMirrorRender();

        // Mirror1 main
        mirror1->update(camera.getPosition());
        mirror1->beginMirrorRender(width, height);
        renderScene(mirror1->getViewMatrix(), mirror1->getProjectionMatrix());
        mirror1->endMirrorRender();

        // Real scene rendering
        glClearColor(background.x, background.y, background.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderScene(camera.getViewMatrix(), projection.getMatrix());
        mirror1->draw(camera.getViewMatrix(), projection.getMatrix());
        mirror2->draw(camera.getViewMatrix(), projection.getMatrix());
    } else {
        std::vector<Mirror *> mirrors;
        mirrors.push_back(&mirror1.value());
        mirrors.push_back(&mirror2.value());

        // Clear mirrors
        for (auto mirror : mirrors) {
            mirror->beginMirrorRender(width, height);
            mirror->endMirrorRender();
        }

        renderMirrorRecursive(mirrors, mirrors[0], width, height, 2, camera.getPosition());
        renderMirrorRecursive(mirrors, mirrors[1], width, height, 2, camera.getPosition());
        // for (Mirror *mirror : mirrors) {
        // renderMirrorRecursive(mirrors, mirror, width, height, 1, camera.getPosition());
        // }

        glClearColor(background.x, background.y, background.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderScene(camera.getViewMatrix(), projection.getMatrix());
        // for (Mirror *mirror : mirrors) {
        //     mirror->draw(camera.getViewMatrix(), projection.getMatrix());
        // }
    }

    std::vector<LayeredMirror *> layeredMirrors;
    layeredMirrors.push_back(&layeredMirror1.value());
    layeredMirrors.push_back(&layeredMirror2.value());
    layeredMirrors.push_back(&layeredMirror3.value());

    renderLayeredMirror(layeredMirrors, layeredMirrors[0], width, height, 0, 4, camera.getPosition());
    renderLayeredMirror(layeredMirrors, layeredMirrors[1], width, height, 0, 4, camera.getPosition());
    renderLayeredMirror(layeredMirrors, layeredMirrors[2], width, height, 0, 4, camera.getPosition());
    // for (Mirror *mirror : mirrors) {
    // renderMirrorRecursive(mirrors, mirror, width, height, 1, camera.getPosition());
    // }

    for (auto mirror : layeredMirrors) {
        mirror->draw(camera.getViewMatrix(), projection.getMatrix());
    }

    skyBox->draw(camera.getViewMatrix(), projection.getMatrix());
}

void MirrorsRenderer::renderScene(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {
    shader.use();
    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projectionMatrix);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
    shader.setMat4("model", model);
    scene->draw(shader);
}

void MirrorsRenderer::renderMirrorRecursive(std::vector<Mirror *> allMirrors, Mirror *mirror, int width, int height,
                                            int depth, const glm::vec3 &viewerPos) {
    if (depth <= 0)
        return;

    mirror->update(viewerPos);

    // First, render other mirrors with depth - 1
    for (Mirror *otherMirror : allMirrors) {
        if (otherMirror != mirror) {
            renderMirrorRecursive(allMirrors, otherMirror, width, height, depth - 1, mirror->getReflectedPos());
        }
    }

    // This is necessary because inner rendering changed the mirror matrices
    mirror->update(viewerPos);

    // Now render THIS mirror, which can safely include the others
    mirror->beginMirrorRender(width, height);
    renderScene(mirror->getViewMatrix(), mirror->getProjectionMatrix());

    for (Mirror *otherMirror : allMirrors) {
        if (otherMirror != mirror) {
            otherMirror->draw(mirror->getViewMatrix(), mirror->getProjectionMatrix());
        }
    }

    mirror->endMirrorRender();
}

void MirrorsRenderer::renderLayeredMirror(std::vector<LayeredMirror *> allMirrors, LayeredMirror *mirror, int width,
                                          int height, int depth, int maxDepth, const glm::vec3 &viewerPos) {
    if (depth >= maxDepth)
        return;

    mirror->update(viewerPos);
    glm::vec3 reflectedPos = mirror->getReflectedCameraPos();
    glm::mat4 mirrorViewMatrix = mirror->getViewMatrix();
    glm::mat4 mirrorProjectionMatrix = mirror->getProjectionMatrix();
    // First, render other mirrors with depth + 1
    for (LayeredMirror *otherMirror : allMirrors) {
        if (otherMirror != mirror) {
            renderLayeredMirror(allMirrors, otherMirror, width, height, depth + 1, maxDepth, reflectedPos);
        }
    }
    mirror->beginMirrorRender(depth, width, height);
    renderScene(mirrorViewMatrix, mirrorProjectionMatrix);

    if (depth < maxDepth - 1) {
        for (auto otherMirror : allMirrors) {
            if (otherMirror != mirror) {
                otherMirror->draw(mirrorViewMatrix, mirrorProjectionMatrix, depth + 1);
            }
        }
    }

    mirror->endMirrorRender(depth);
}
