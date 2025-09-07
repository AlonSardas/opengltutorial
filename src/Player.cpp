#include "Player.h"

void Player::updateViewMatrix() {
    glm::vec3 lookingDirection = front;
    glm::vec3 cameraOffset = 2.0f * up - 3.0f * front;

    viewMatrix = glm::lookAt(position + cameraOffset, position + cameraOffset + lookingDirection, worldUp);
}
