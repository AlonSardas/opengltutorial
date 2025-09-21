#pragma once

#include "graphics/DepthCubeMap.h"
#include "graphics/Shader.h"
#include <glm/glm.hpp>

class PointLight {
  public:
    PointLight(const glm::vec3 &pos = glm::vec3(0.0f), float nearP = 1.0f, float farP = 25.0f)
        : position(pos), nearPlane(nearP), farPlane(farP),
          depthCubMapShader("shaders/depthMap/depthCubeMap.vs", "shaders/depthMap/depthCubeMap.fs",
                            "shaders/depthMap/depthCubeMap.gs") {
        depthCubMapShader.setUsesTexture(false);
    }
    ~PointLight() = default;
    PointLight(const PointLight &) = delete;
    PointLight &operator=(const PointLight &) = delete;
    PointLight(PointLight &&) = delete;
    PointLight &operator=(PointLight &&) = delete;

    void beginDepthPass(int width, int height);
    void endDepthPass();
    Shader &getShader() { return depthCubMapShader; }

    glm::vec3 position;
    float nearPlane, farPlane;
    DepthCubeMap depthCubeMap;

  private:
    Shader depthCubMapShader;
};