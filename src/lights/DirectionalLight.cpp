#include "DirectionalLight.h"
#include <glm/gtc/matrix_transform.hpp>

void DirectionalLight::beginDepthPass(int width, int height) {
    // render scene from light's point of view
    depthMapShader.use();
    depthMapShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    depthMap.bind(0, 0, width, height);
    depthMap.clear();
}

void DirectionalLight::endDepthPass() { depthMap.unbind(); }

void DirectionalLight::updateLightSpaceMatrix() {
    glm::mat4 lightProjection, lightView;
    lightProjection = glm::ortho(-frustumLimit, frustumLimit, -frustumLimit, frustumLimit, nearPlane, farPlane);
    lightView = glm::lookAt(position, position + direction, glm::vec3(1.0, 0.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
}
