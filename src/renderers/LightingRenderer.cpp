/*
https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/1.colors/colors.cpp

https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/5.2.light_casters_point/light_casters_point.cpp
*/

#include "LightingRenderer.h"
#include "Cube.h"
#include <GLFW/glfw3.h>

LightingRenderer::LightingRenderer(const QuatCamera &c, PerspectiveProjection &proj)
    : lightingShader("shaders/transformNormalsTexture.vs", "shaders/materialLightFragment.fs"),
      // lightingShader("shaders/transformAndNormals.vs", "shaders/materialLightFragment.fs"),
      lightCubeShader("shaders/transform.vs", "shaders/solidLightSource.fs"), camera(c), projection(proj),
      lightPos(1.2f, 1.0f, 2.0f) {
    glGenVertexArrays(1, &lightVAO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

LightingRenderer::~LightingRenderer() {
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void LightingRenderer::onResize(int width, int height) { glViewport(0, 0, width, height); }

void LightingRenderer::init() {
    glEnable(GL_DEPTH_TEST);

    const auto vertices = getCubeVerticesWithNormalsAndTexture();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already
    // contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    texture.bind(0);
    texture.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    texture.setMinMagFilters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    texture.loadImage("resources/container2.png", false);
    textureSpecular.bind(1);
    textureSpecular.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    textureSpecular.setMinMagFilters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    textureSpecular.loadImage("resources/container2_specular.png", false);

    // don't forget to use the corresponding shader program first (to set the uniform)
    lightingShader.use();
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    lightingShader.setFloat("light.constant", 1.0f);
    lightingShader.setFloat("light.linear", 0.09f);
    lightingShader.setFloat("light.quadratic", 0.032f);

    lightingShader.setVec3("directionalLight.direction", -0.2f, -1.0f, -0.3f);
    lightingShader.setVec3("directionalLight.ambient", glm::vec3(0.1f));
    lightingShader.setVec3("directionalLight.diffuse", glm::vec3(0.5f));
    lightingShader.setVec3("directionalLight.specular", glm::vec3(0.2f));

    lightingShader.setFloat("flashlight.cutOff", glm::cos(glm::radians(12.5f)));
    lightingShader.setFloat("flashlight.outerCutOff", glm::cos(glm::radians(17.5f)));
    lightingShader.setVec3("flashlight.ambient", 0.1f, 0.1f, 0.1f);
    lightingShader.setVec3("flashlight.diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.setVec3("flashlight.specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setFloat("flashlight.constant", 1.0f);
    lightingShader.setFloat("flashlight.linear", 0.09f);
    lightingShader.setFloat("flashlight.quadratic", 0.032f);
}

void LightingRenderer::render() {
    glClearColor(background.x, background.y, background.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lightingShader.use();
    lightingShader.setMat4("view", camera.getViewMatrix());
    lightingShader.setMat4("projection", projection.getMatrix());
    lightingShader.setVec3("viewPos", camera.getPosition());

    glm::vec3 lightColor;
    bool shouldChangeLightColor = false;
    if (shouldChangeLightColor) {
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);
    } else {
        lightColor = glm::vec3(1.0f);
    }
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.8f);
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.3f);

    lightingShader.setVec3("light.ambient", ambientColor);
    lightingShader.setVec3("light.diffuse", diffuseColor);

    glBindVertexArray(VAO);

    auto cubePositions = getCubePositions();
    for (unsigned int i = 0; i < 10; i++) {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        lightingShader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    float timeValue = glfwGetTime();
    float velocity = 20.02f;

    auto model = glm::mat4(1.0f);
    glm::vec3 baseLightPos = {2.2f, 1.0f, 2.0f};
    bool shouldMoveLightSource = true;
    if (shouldMoveLightSource) {
        model = glm::rotate(model, glm::radians(timeValue * velocity), glm::vec3(0.0, 1.0, 0.0));
        model = glm::translate(model, baseLightPos);
        model = glm::rotate(model, glm::radians(timeValue * velocity * 3), glm::vec3(0.0, 1.0, 0.0));
    } else {
        model = glm::translate(model, baseLightPos);
    }
    model = glm::scale(model, glm::vec3(0.3f));
    lightPos = model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    lightingShader.setVec3("light.position", lightPos);

    lightingShader.setVec3("flashlight.position", camera.getPosition());
    lightingShader.setVec3("flashlight.direction", camera.getFront());

    lightCubeShader.use();
    lightCubeShader.setVec3("color", lightColor);
    lightCubeShader.setMat4("model", model);
    lightCubeShader.setMat4("view", camera.getViewMatrix());
    lightCubeShader.setMat4("projection", projection.getMatrix());

    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

const std::array<glm::vec3, 10> &LightingRenderer::getCubePositions() {
    static const std::array<glm::vec3, 10> cubePositions = {
        glm::vec3(0.0f, 0.0f, 0.0f),     glm::vec3(2.0f, 5.0f, -15.0f), glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f), glm::vec3(2.4f, -0.4f, -3.5f), glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),   glm::vec3(1.5f, 2.0f, -2.5f),  glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};
    return cubePositions;
}
