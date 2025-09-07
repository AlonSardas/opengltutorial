#include "QuatCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

QuatCamera::QuatCamera(glm::vec3 startPos, float moveSpeed, float turnSpeed)
    : BaseControllable(moveSpeed, turnSpeed), position(startPos), orientation(glm::quat(1, 0, 0, 0)) {
    updateViewMatrix();
}

void QuatCamera::rotate(float yawDegrees, float pitchDegrees, float rollDegrees) {
    glm::quat qYaw = glm::angleAxis(glm::radians(-yawDegrees), orientation * glm::vec3(0, 1, 0));
    glm::quat qPitch = glm::angleAxis(glm::radians(pitchDegrees), orientation * glm::vec3(1, 0, 0));
    glm::quat qRoll = glm::angleAxis(glm::radians(rollDegrees), orientation * glm::vec3(0, 0, 1));
    orientation = normalize(qYaw * qPitch * qRoll * orientation);
    front = orientation * glm::vec3(0, 0, -1);

    updateViewMatrix();
}

void QuatCamera::move(const glm::vec3 &delta) {
    glm::vec3 right = orientation * glm::vec3(1, 0, 0);
    position += right * delta.x;

    glm::vec3 up = orientation * glm::vec3(0, 1, 0);
    position += up * delta.y;

    glm::vec3 forward = orientation * glm::vec3(0, 0, -1);
    position += forward * delta.z;
    updateViewMatrix();
}

const glm::vec3 &QuatCamera::getFront() const { return front; }

void QuatCamera::updateViewMatrix() {
    // std::cout << "Camera Position " << position.x << ", " << position.y << ", " << position.z << std::endl;

    // Rotation matrix from orientation quaternion
    glm::mat4 rot = glm::mat4_cast(glm::conjugate(orientation));

    // Translation matrix inverse (move world opposite to camera)
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), -position);

    // View matrix = rotation * translation
    viewMatrix = rot * trans;
}
