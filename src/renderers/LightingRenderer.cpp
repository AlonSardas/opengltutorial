/*
https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/1.colors/colors.cpp
*/

#include "LightingRenderer.h"
#include "Cube.h"
#include <GLFW/glfw3.h>

LightingRenderer::LightingRenderer(const QuatCamera &c,
                                   PerspectiveProjection &proj)
    : lightingShader("shaders/transformAndNormals.vs",
                     "shaders/lightFragment.fs"),
      lightCubeShader("shaders/transform.vs", "shaders/solidLightSource.fs"),
      camera(c), projection(proj), lightPos(1.2f, 1.0f, 2.0f) {
    glGenVertexArrays(1, &lightVAO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

LightingRenderer::~LightingRenderer() {
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void LightingRenderer::onResize(int width, int height) {
    glViewport(0, 0, width, height);
}

void LightingRenderer::init() {
    glEnable(GL_DEPTH_TEST);

    const auto vertices = getCubeVerticesWithNormals();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already
    // contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // don't forget to use the corresponding shader program first (to set the
    // uniform)
    lightingShader.use();
    lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
}

void LightingRenderer::render() {
    glClearColor(0.0f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.0f));
    // model =
    //     glm::rotate(model, glm::radians(-98.0f), glm::vec3(1.0f, 0.0f,
    //     0.0f));
    // model =
    //     glm::rotate(model, glm::radians(-98.0f), glm::vec3(0.0f,
    //     0.0f, 1.0f));

    lightingShader.use();
    lightingShader.setMat4("model", model);
    lightingShader.setMat4("view", camera.getViewMatrix());
    lightingShader.setMat4("projection", projection.getMatrix());
    lightingShader.setVec3("viewPos", camera.getPosition());

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    float timeValue = glfwGetTime();
    float velocity = 20.02f;

    model = glm::mat4(1.0f);
    glm::vec3 baseLightPos = {1.2f, 1.0f, 2.0f};
    model = glm::rotate(model, glm::radians(timeValue * velocity),
                        glm::vec3(0.0, 1.0, 0.0));
    model = glm::translate(model, baseLightPos);
    model = glm::rotate(model, glm::radians(timeValue * velocity * 3),
                        glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale(model, glm::vec3(0.3f));
    lightPos = model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    lightingShader.setVec3("lightPos", lightPos);

    lightCubeShader.use();
    lightCubeShader.setMat4("model", model);
    lightCubeShader.setMat4("view", camera.getViewMatrix());
    lightCubeShader.setMat4("projection", projection.getMatrix());

    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
