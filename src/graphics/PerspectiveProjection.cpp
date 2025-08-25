#include "PerspectiveProjection.h"
#include <algorithm>
#include <glm/gtc/constants.hpp>

PerspectiveProjection::PerspectiveProjection(float fovDegrees, float aspect,
                                             float nearP, float farP)
    : fov(fovDegrees), aspectRatio(aspect), nearPlane(nearP), farPlane(farP) {
    updateProjectionMatrix();
}

void PerspectiveProjection::setFov(float degrees) {
    fov = std::clamp(degrees, MIN_FOV, MAX_FOV);
    updateProjectionMatrix();
}

void PerspectiveProjection::adjustFov(float deltaDegrees) {
    setFov(fov + deltaDegrees);
}

float PerspectiveProjection::getFov() const { return fov; }

void PerspectiveProjection::setAspectRatio(float ratio) {
    aspectRatio = ratio;
    updateProjectionMatrix();
}

float PerspectiveProjection::getAspectRatio() const { return aspectRatio; }

void PerspectiveProjection::setClippingPlanes(float nearP, float farP) {
    nearPlane = nearP;
    farPlane = farP;
    updateProjectionMatrix();
}

glm::mat4 PerspectiveProjection::getMatrix() const { return projection; }

void PerspectiveProjection::updateProjectionMatrix() {
    projection =
        glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}