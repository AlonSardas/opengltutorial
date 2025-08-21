#include "StencilGlowExample.h"

StencilGlowExample::StencilGlowExample(const QuatCamera &c, PerspectiveProjection &proj)
    : camera(c), projection(proj), shader("shaders/transform.vs", "shaders/solidColor.fs"),
      textureShader("shaders/vertexTransformer2.vs", "shaders/fragmentAdapterWithTexture.fs"),
      stencilShader("shaders/transform.vs", "shaders/solidColor.fs"), color(0.7f, 0.2f, 0.2f) {}

StencilGlowExample::~StencilGlowExample() {}

void StencilGlowExample::init() {
    glEnable(GL_DEPTH_TEST);
    cube1.emplace();
    cube2.emplace();
    cube3.emplace();

    cube3->setTexture("resources/Pitz1.jpeg");

    shader.use();
    shader.setVec3("color", glm::vec3(0.2f, 0.5f, 0.7f));
    stencilShader.use();
    stencilShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void StencilGlowExample::render() {

    glClearColor(background.x, background.y, background.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Avoid further stencil writings
    glStencilMask(0x00);

    shader.use();
    shader.setMat4("view", camera.getViewMatrix());
    shader.setMat4("projection", projection.getMatrix());

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down
    shader.setMat4("model", model);

    cube1->draw(shader);

    // Record the drawn object in stencil buffer
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));     // it's a bit too big for our scene, so scale it down
    shader.setMat4("model", model);
    cube2->draw(shader);

    // Now there are 1s in the cube pixels
    // We don't want to draw on it again
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    stencilShader.use();
    // stencilShader.setVec3("color", )
    float scale = 1.1f;
    glm::mat4 scaled = glm::scale(model, glm::vec3(scale, scale, scale));
    stencilShader.setMat4("model", scaled);
    stencilShader.setMat4("view", camera.getViewMatrix());
    stencilShader.setMat4("projection", projection.getMatrix());
    cube2->draw(stencilShader);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);

    textureShader.use();
    textureShader.setMat4("view", camera.getViewMatrix());
    textureShader.setMat4("projection", projection.getMatrix());

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-2.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.8f, 0.8f, 1.5f));
    shader.setMat4("model", model);
    cube3->draw(textureShader);
}

void StencilGlowExample::onResize(int width, int height) { glViewport(0, 0, width, height); }
