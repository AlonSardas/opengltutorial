#pragma once

#include "EulerCamera.h"
#include "QuatCamera.h"
#include "Shader.h"
#include "Texture.h"
#include <GLFW/glfw3.h>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Renderer {
  public:
    Renderer(const QuatCamera &c);
    ~Renderer();
    void init();
    void render();
    void onResize(int width, int height);
    void adjustFov(float deltaDegrees);

  private:
    const QuatCamera &camera;
    Shader shader;
    unsigned int VBO, VAO, EBO;
    glm::mat4 model, view, projection;
    Texture texture1, texture2;
    float aspectRatio;
    float fov = 45.0f;
    static constexpr float MIN_FOV = 1.0f;
    static constexpr float MAX_FOV = 70.0f;

    void updateProjectionMatrix();

    static const std::array<glm::vec3, 10> &getCubePositions();
    static const std::array<float, 180> &getCubeVertices();
};