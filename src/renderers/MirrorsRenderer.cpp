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
    mirror1.emplace();
    // TODO: Fix
    mirror1->init(glm::vec3(20.0f, 4.0f, -30.0f), glm::vec3(1.0f, 0.0f, -0.0f), 4, 6, 500, 500);
    stbi_set_flip_vertically_on_load(false);
    scene.emplace("../models/Apocalyptic City/Apocalyptic City.obj");

    projection.setClippingPlanes(0.1f, 1000.0f);
}

void MirrorsRenderer::render() {
    // First render the mirror
    framebuffer->bind(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    // make sure we clear the framebuffer's content
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // camera.rotateRight(180);
    // shader.setMat4("view", camera.getViewMatrix());
    // camera.rotateLeft(180);
    // projection.setAspectRatio((float)framebuffer->getWidth() / framebuffer->getHeight());
    // shader.setMat4("projection", projection.getMatrix());

    // First scene rendering
    renderScene();

    framebuffer->unbind();

    mirror1->update(camera.getPosition(), camera.getViewMatrix(), projection.getMatrix());
    mirror1->beginMirrorRender(width, height);
    shader.use();
    shader.setMat4("view", mirror1->getViewMatrix());
    shader.setMat4("projection", mirror1->getProjectionMatrix());
    renderScene();
    mirror1->endMirrorRender();

    // second render pass: draw as normal
    // ----------------------------------

    projection.setAspectRatio(width / height);
    glClearColor(background.x, background.y, background.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.setMat4("view", camera.getViewMatrix());
    shader.setMat4("projection", projection.getMatrix());

    // second scene rendering
    renderScene();

    // draw mirror
    // glDisable(GL_DEPTH_TEST);
    // screenShader.use();
    // // glBindTexture(GL_TEXTURE_2D, framebuffer->getTexture());
    // glBindTexture(GL_TEXTURE_2D, mirror1->getTexture());
    // screenQuad->draw(screenShader);
    // mirror1->draw();
}

void MirrorsRenderer::renderScene() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
    shader.setMat4("model", model);
    scene->draw(shader);
}