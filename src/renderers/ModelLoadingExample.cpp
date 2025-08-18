#include "ModelLoadingExample.h"
#include "stb/stb_image.h"

ModelLoadingExample::ModelLoadingExample(const QuatCamera &c, PerspectiveProjection &proj)
    : camera(c), projection(proj),
      shader("shaders/vertexTransformer2.vs", "shaders/fragmentAdapterWithTexture.fs"
             //   shader("../modelloadertest/1.model_loading.vs", "../modelloadertest/1.model_loading.fs"
      ) {}

ModelLoadingExample::~ModelLoadingExample() {}

void ModelLoadingExample::init() {
    glEnable(GL_DEPTH_TEST);

    stbi_set_flip_vertically_on_load(true);
    ourModel.emplace("../models/backpack/backpack.obj");
    cube.emplace();
    cube2.emplace();
}

void ModelLoadingExample::render() {
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    shader.setMat4("view", camera.getViewMatrix());
    shader.setMat4("projection", projection.getMatrix());

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down
    shader.setMat4("model", model);

    cube->draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));     // it's a bit too big for our scene, so scale it down
    shader.setMat4("model", model);
    cube2->draw(shader);

    ourModel->draw(shader);
}

void ModelLoadingExample::onResize(int width, int height) { glViewport(0, 0, width, height); }
