#include "EulerCamera.h"
#include <glm/gtc/matrix_transform.hpp>

EulerCamera::EulerCamera(glm::vec3 position, float yaw, float pitch)
    : position(position), yaw(yaw), pitch(pitch), worldUp(0.0f, 1.0f, 0.0f) {
    updateCameraVectors();
    updateViewMatrix();
}

void EulerCamera::moveForward(float amount) {
    position += front * amount;
    updateViewMatrix();
}

void EulerCamera::moveRight(float amount) {
    position += right * amount;
    updateViewMatrix();
}

void EulerCamera::moveUp(float amount) {
    position += worldUp * amount;
    updateViewMatrix();
}

void EulerCamera::rotate(float yawOffset, float pitchOffset) {
    yaw += yawOffset;
    pitch += pitchOffset;

    // Clamp pitch to avoid gimbal lock
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    updateCameraVectors();
    updateViewMatrix();
}

void EulerCamera::updateCameraVectors() {
    // Convert spherical to Cartesian
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void EulerCamera::updateViewMatrix() {
    viewMatrix = glm::lookAt(position, position + front, up);
}
