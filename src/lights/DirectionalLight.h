#pragma once

#include "graphics/DepthCubeMap.h"
#include "graphics/Shader.h"
#include <glm/glm.hpp>

class DirectionalLight {
  public:
    DirectionalLight(const glm::vec3 &dir = glm::vec3(0.0f, -1.0f, 0.0f), float frustumLim = 20.0f, float nearP = 0.01f,
                     float farP = 400.0f)
        : direction(dir), frustumLimit(frustumLim), nearPlane(nearP), farPlane(farP),
          depthMapShader("shaders/depthMap/depthShader.vs", "shaders/emptyFragment.fs") {
        depthMapShader.setUsesTexture(false);
    }
    ~DirectionalLight() = default;
    DirectionalLight(const DirectionalLight &) = delete;
    DirectionalLight &operator=(const DirectionalLight &) = delete;
    DirectionalLight(DirectionalLight &&) = delete;
    DirectionalLight &operator=(DirectionalLight &&) = delete;

    void beginDepthPass(int width, int height);
    void endDepthPass();
    Shader &getShader() { return depthMapShader; }
    void updateLightSpaceMatrix();
    const glm::mat4 getLightSpaceMatrix() const { return lightSpaceMatrix; }

    glm::vec3 direction;
    glm::vec3 position; // This is used for the projection matrix calculation
    float frustumLimit, nearPlane, farPlane;
    DepthMap depthMap;

  private:
    Shader depthMapShader;
    glm::mat4 lightSpaceMatrix;
};