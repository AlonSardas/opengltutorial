#include "POVPlayer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

POVPlayer::POVPlayer(const glm::vec3 &startPos, float moveSpeed, float turnSpeed)
    : BaseControllable(moveSpeed, turnSpeed), position(startPos), worldUp(0.0f, 1.0f, 0.0f), yaw(0), pitch(0),
      portalComponent(this) {
    updateVectors();
    updateViewMatrix();
}

void POVPlayer::rotate(float yawDegrees, float pitchDegrees, float rollDegrees) {
    yaw += yawDegrees;
    pitch += pitchDegrees;

    // Clamp pitch to avoid gimbal lock
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    updateVectors();
    updateViewMatrix();
}

void POVPlayer::updateVectors() {
    // Convert spherical to Cartesian
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(direction);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void POVPlayer::move(const glm::vec3 &delta) {
    position += right * delta.x;
    // position += up * delta.y;
    glm::vec3 frontMovement = front;
    frontMovement.y = 0; // Prevent flying when looking up/down
    frontMovement = glm::normalize(frontMovement);
    position += frontMovement * delta.z;

    if (portalComponent.checkPortals()) {
        std::cout << "Got teleported" << std::endl;
        updateVectors();
        updateViewMatrix();
    } else {
        updateViewMatrix();
    }
}

void POVPlayer::updateViewMatrix() { viewMatrix = glm::lookAt(position, position + front, up); }
